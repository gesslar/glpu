/**
 * @file /adm/daemons/httpc.c
 * @description HTTP client daemon to fetch data from the web
 *              and return it to the caller via callback
 *
 * @created 2024/07/06 - Gesslar
 * @last_modified 2024/07/06 - Gesslar
 *
 * @history
 * 2024/07/06 - Gesslar - Created
 */

inherit STD_HTTP_CLIENT ;

private nosave mapping serials = ([]) ;

void setup() {
    set_log_prefix("(HTTPC)") ;
    set_log_level(1) ;
}

varargs int fetch(mixed *callback, string method, string url, mapping headers, string body) {
    int serial ;
    mapping request ;

    _log(2, "Fetching " + url) ;

    if(!mapp(request = http_request(url, method, headers, body)))
        error("Error creating request") ;

    if(nullp(callback))
        error("No callback specified") ;

    if(!stringp(url))
        error("Invalid URL") ;

    serial = request["start_time"] ;
    serials[serial] = callback ;

    _log(3, "Recording serial %d for callback: %O %O", serial, callback, previous_object()) ;

    return serial ;
}

nomask void handle_shutdown(mapping server) {
    int serial ;
    mixed *callback ;
    mixed func ;
    object ob ;
    mixed err ;

    _log(2, "Handling shutdown") ;

    serial = server["request"]["start_time"] ;
    if(!serials[serial]) {
        _log(3, "No callback found for serial %d", serial) ;
        return ;
    }

    callback = serials[serial] ;

    err = catch(call_back(callback, server["response"])) ;
    if(err)
        _log(2, "Error executing callback: %O", err) ;
    else
        _log(3, "Callback executed") ;
}
