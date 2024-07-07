/**
 * @file /adm/daemons/github_issues.c
 * @description Daemon to handle github issues
 * @details This daemon is used to handle GitHub issues for the mud. It uses
 * the GitHub Issues API to create issues in the mud's GitHub repository.
 *
 * @created 2024/07/07 - Gesslar
 * @last_modified 2024/07/07 - Gesslar
 *
 * @history
 * 2024/07/07 - Gesslar - Created
 */

#include <daemons.h>

inherit STD_HTTP_CLIENT ;

private nomask int parse_time(string str) ;
private nomask int is_setup() ;
private nomask void check_token() ;
nomask void process_backlog() ;
private nomask void process_next(string *files) ;
private nomask void execute_callback(mapping request, mapping response) ;

private nomask nosave string GH_OWNER_LABEL = "GITHUB_REPORTER_OWNER" ;
private nomask nosave string GH_REPO_LABEL = "GITHUB_REPORTER_REPO" ;
private nomask nosave string GH_PAT_LABEL = "GITHUB_REPORTER_PAT" ;
private nomask nosave string GH_TYPES_LABEL = "GITHUB_REPORTER_TYPES" ;

private nomask nosave mapping api_calls = ([ ]) ;

void setup() {
    set_log_level(2) ;

    assure_dir("/data/github/issues/pending") ;

    process_backlog() ;
}

/**
 * @description Create a new issue in the GitHub repository
 * @param {string} type - The type of issue to create
 * @param {string} title - The title of the issue
 * @param {string} body - The body of the issue
 * @param {mixed} callback - The callback function to call when the request is
 *                           complete
 * @return int - Returns 1 if the request was successful, 0 otherwise
*/

varargs int create_issue(string type, string title, string body, mixed callback) {
    string owner = mud_config(GH_OWNER_LABEL) ;
    string repo = mud_config(GH_REPO_LABEL) ;
    string token = mud_config(GH_PAT_LABEL) ;
    string *types = mud_config(GH_TYPES_LABEL) ;
    string url ;
    mapping request ;

    if(!stringp(type))
        error("Type must be a string") ;

    if(member_array(type, types) == -1)
        error("Invalid type. Available types: " + implode(types, ", ")) ;

    if(!stringp(title))
        error("Title must be a string") ;

    if(!stringp(body))
        error("Body must be a string") ;

    if(!valid_function(callback))
        if(!stringp(callback))
            error("Callback must be a string or a function") ;
        else
            if(!function_exists(callback, previous_object()))
                error("Callback function does not exist") ;

    if(!owner || !repo || !token) {
        _log(2, "GitHub Issues API not setup in adm/etc/config.json") ;
        return 0 ;
    }

    url = sprintf("https://api.github.com/repos/%s/%s/issues", owner, repo) ;

    request = http_request(
        url,
        "POST",
        ([
            "Authorization": sprintf("token %s", token),
            "Accept": "application/vnd.github.v3+json",
            "Content-Type": "application/json"
        ]),
        json_encode(([
            "title": title,
            "body": "```\n" + body + "\n```\n",
            "labels": ({ type })
        ])),
    ) ;

    api_calls[request["start_time"]] = ([
        "type" : "create",
        "label" : type,
        "title" : title,
        "body" : body,
        "callback" : callback,
        "caller" : previous_object(),
        "request" : request
    ]) ;

    return mapp(request) ? 1 : 0 ;
}

/**
 * @description Handle the response from the GitHub API
 * @param {mapping} response - The response from the GitHub API
 * @return {void}
*/
void handle_shutdown(mapping response) {
    mapping request ;
    mixed body, type ;
    int request_id ;
    mixed err ;

    body = response["response"]["body"] ;

    request_id = response["request"]["start_time"] ;
    if(!request = api_calls[request_id]) {
        _log(2, "No request found for id %d", request_id) ;
        return ;
    }
    map_delete(api_calls, request_id) ;

    if(response["response"]["status"]["code"] != 201) {
        string file ;
        _log(2, "Request error: %d %s\nO",
            response["response"]["status"]["code"],
            response["response"]["status"]["message"],
            body
        ) ;
        file = time_ns() + ".txt" ;
        write_file("/data/github/issues/pending/" + file, save_variable(request)) ;
        _log(2, "Request saved to /data/github/pending/%s", file) ;
        return ;
    }

    _log(2, "Content-Type: %O", response["response"]["headers"]["content-type"]) ;

    body = parse_body(body, response["response"]["headers"]["content-type"]) ;

    _log(2, "Body:\n%O", body) ;

    err = catch {
        if(request["callback"]) {
            if(stringp(request["callback"]) && request["callback"] != "RESUBMIT")
                if(function_exists(request["callback"], request["caller"]))
                    call_other(request["caller"], request["callback"], body) ;
            else if(valid_function(request["callback"]))
                (*request["callback"])(body) ;
        }
    } ;

    if(err)
        _log(2, "Error calling callback: %O", err) ;
}

/**
 * @description Execute the callback function
 * @param {mapping} request - The request containing the callback function
 * @param {mapping} response - The response from the GitHub API
 * @return {void}
*/
void execute_callback(mapping request, mapping response) {
    mixed err ;

    err = catch {
        if(request["callback"]) {
            if(stringp(request["callback"]) && request["callback"] != "RESUBMIT") {
                if(objectp(request["caller"]) && function_exists(request["callback"], request["caller"])) {
                    call_other(request["caller"], request["callback"], response) ;
                } else {
                    _log(2, "Invalid caller or callback function: %O %O", request["caller"], request["callback"]) ;
                }
            } else if(valid_function(request["callback"])) {
                (*request["callback"])(response) ;
            } else {
                _log(2, "Invalid callback function: %O", request["callback"]) ;
            }
        }
    } ;

    if(err)
        _log(2, "Error calling callback: %O", err) ;
}

/**
 * @description Process the backlog of requests
 * @details This function processes the backlog of requests in the
 *          /data/github/issues/pending directory. It passes a list of files to
 *          process_next() to process each request.
 * @return {void}
*/
public nomask void process_backlog() {
    string *files ;

    files = get_dir("/data/github/issues/pending/*.txt") ;
    if(!sizeof(files))
        return ;

    process_next(files) ;
}

/**
 * @description Process the next request in the backlog
 * @param {string[]} files - The list of files remaining in the backlog to process
 * @return {void}
*/
private nomask void process_next(string *files) {
    string file ;
    mapping request ;
    mixed err ;
    function schedule_next = (: call_out_walltime("process_next", 5.0, $1) :) ;

    if(!sizeof(files))
        return ;

    file = files[0] ;

    if(!file_exists(file)) {
        if(sizeof(files) > 1)
            (*schedule_next)(files[1..]) ;
        else
            return ;
    }

    err = catch(request = restore_variable(read_file("/data/github/issues/pending/" + file))) ;
    if(err) {
        _log(2, "Error reading request file %s: %O", file, err) ;
        if(sizeof(files) > 1)
            (*schedule_next)(files[1..]) ;
        return ;
    }

    if(!mapp(request)) {
        _log(2, "Invalid request in /data/github/issues/pending/%s", file) ;
        if(sizeof(files) > 1)
            (*schedule_next)(files[1..]) ;
        return ;
    }

    if(!mapp(request["request"])) {
        _log(2, "Invalid request in /data/github/issues/pending/%s", file) ;
        return ;
    }

    _log(2, "Processing request from /data/github/issues/pending/%s", file) ;

    _log(2, "Request: %O", request) ;

    _log(2, "Resubmitting request from /data/github/issues/pending/%s", file) ;

    // Do appropriate function based on its type
    err = catch {
        switch(request["type"]) {
            case "create": {
                mixed *new_request = ({
                    request["label"],
                    request["title"],
                    request["body"],
                    request["callback"]
                }) ;
                create_issue(new_request...) ;
                _log(2, "Submitting create issue request: %O", new_request) ;
                break ;
            }
            default:
                _log(2, "Invalid request type: %s", request["type"]) ;
                break ;
        }
    } ;

    if(err)
        _log(2, "Error sending request: %O", err) ;
    else {
        rm("/data/github/issues/pending/" + file) ;
        if(sizeof(files) > 1)
            (*schedule_next)(files[1..]) ;
        _log(2, "Request resubmitted for /data/github/issues/pending/%s", file) ;
    }
}
/**
 * @description convert a GitHub date-time string to an epoch time
 * @param {string} datetime - The date-time string to convert
 * @return {int} - The epoch time
 */
private nomask int parse_time(string datetime) {
    int epoch_time;
    mixed err ;

    // Convert the date-time string to a time structure using strptime
    err = catch(epoch_time = strptime("%Y-%m-%dT%H:%M:%SZ", datetime)) ;
    if(err) {
        _log(0, "Error parsing date-time string %s: %O", datetime, err) ;
        return -1 ;
    }

    // If strptime returns -1, it means the date-time string is invalid
    if (!epoch_time) {
        error("Invalid date-time string");
    }

    return epoch_time;
}
