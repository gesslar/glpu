/**
 * @file /adm/daemons/github_issues.c
 * @description Daemon to handle github issues
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

private nomask nosave string INSTALLATION_ID_LABEL = "GITHUB_REPORTER_INSTALLATION_ID" ;
private nomask nosave string JWT_LABEL = "GITHUB_REPORTER_JWT" ;
private nomask nosave string INSTALLATION_ID, JWT ;
private nomask nosave string TOKEN ;
private nomask nosave int EXPIRES ;
private nomask nosave mapping token_requests = ([ ]) ;

void setup() {
    mapping swap ;

    set_log_level(4) ;

    swap = SWAP_D->swap_out("github_issues") ;
    if(swap) {
        INSTALLATION_ID = swap["INSTALLATION_ID"] ;
        JWT = swap["JWT"] ;
        TOKEN = swap["TOKEN"] ;
        EXPIRES = swap["EXPIRES"] ;
    }

    if(!stringp(INSTALLATION_ID)) ENV_D->fetch(INSTALLATION_ID_LABEL, "set_appid", 0) ;
    if(!stringp(JWT)) ENV_D->fetch(JWT_LABEL, "set_jwt", 0) ;
}

int fetch_token() {
    string url = "https://api.github.com/app/installations/1/access_tokens" ;
    string body = "{\"repository_ids\":[1]}" ;
    mapping headers = ([
        "Accept": "application/vnd.github.v3+json",
        "Authorization": "Bearer [1]",
        "User-Agent": get_config(__MUD_NAME__),
    ]) ;
    mapping request ;

    if(!is_setup()) {
        _log(2, "Installation ID or JWT not set") ;
        return 0 ;
    }

    _log(2, "Fetching token") ;

    body = replace_string(body, "[1]", INSTALLATION_ID) ;
    headers["Authorization"] = replace_string(headers["Authorization"], "[1]", JWT) ;
    request = http_request(url, "POST", headers, body) ;

    if(!mapp(request)) {
        _log(2, "Error fetching token") ;
        return 0 ;
    }

    token_requests[request["start_time"]] = request ;

    return mapp(request) ? 1 : 0 ;
}

void handle_shutdown(mapping response) {
    mapping request ;
    mapping body ;
    int request_id ;

    body = response["response"]["body"] ;

    if(response["response"]["status"]["code"] != 201) {
        _log(2, "Error fetching token: %d %s\n%O",
            response["response"]["status"]["code"],
            response["response"]["status"]["message"],
            body
        ) ;
        return ;
    }

    request_id = response["start_time"] ;
    if(!request = token_requests[request_id]) {
        _log(2, "No request found for id %d\n", request_id) ;
        return ;
    }

    TOKEN = response["response"]["body"]["token"] ;
    EXPIRES = parse_time(response["response"]["body"]["expires_at"]) ;

    _log(2, "Token: %s\n", TOKEN) ;
    _log(2, "Expires: %d\n", EXPIRES) ;

    map_delete(token_requests, request_id) ;
}

private nomask int parse_time(string datetime) {
    int epoch_time;
    mixed err ;

    // Convert the date-time string to a time structure using strptime
    err = catch(epoch_time = strptime("%Y-%m-%dT%H:%M:%SZ", datetime)) ;
    if(err) {
        _log(0, "Error parsing date-time string %s: %O\n", datetime, err) ;
        return -1 ;
    }

    // If strptime returns -1, it means the date-time string is invalid
    if (!epoch_time) {
        error("Invalid date-time string");
    }

    return epoch_time;
}

int remove() {
    SWAP_D->swap_in("github_issues", ([
        "INSTALLATION_ID": INSTALLATION_ID,
        "JWT": JWT,
        "TOKEN": TOKEN,
        "EXPIRES": EXPIRES
    ])) ;

    return ::remove() ;
}

void set_appid(string appid) {
    _log(2, "Setting appid: %s\n", appid) ;
    INSTALLATION_ID = appid ;
}
void set_jwt(string jwt) {
    JWT = jwt ;
    _log(2, "Setting jwt: %s\n", jwt) ;
}

private nomask int check_token() {
    if(!is_setup())
        return -1 ;

    if(!stringp(TOKEN) || !EXPIRES)
        return 0 ;

    if(EXPIRES < time()) {
        return 0 ;
    }

    return 1 ;
}

private nomask int is_setup() { return stringp(INSTALLATION_ID) && stringp(JWT) ; }
