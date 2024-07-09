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

int fetch(mixed callback, string url, mapping headers) {
    int serial ;
    mapping request ;

    _log(2, "Fetching " + url) ;

    if(!mapp(request = http_request(url, "GET", headers)))
        error("Error creating request") ;

    if(nullp(callback))
        error("No callback specified") ;

    if(!stringp(url))
        error("Invalid URL") ;

    if(!valid_function(callback)) {
        object ob = previous_object() ;
        if(!function_exists(callback, ob))
            error("Invalid callback") ;
    }

    serial = request["start_time"] ;
    serials[serial] = ({ callback, previous_object() }) ;

    _log(3, "Recording serial %d for callback: %O %O", serial, callback, previous_object()) ;

    return serial ;
}

nomask void handle_shutdown(mapping server) {
    int serial ;
    mixed *callback_info ;
    mixed func ;
    object ob ;
    mixed err ;

    _log(2, "Handling shutdown") ;

    serial = server["request"]["start_time"] ;
    if(!serials[serial]) {
        _log(3, "No callback found for serial %d", serial) ;
        return ;
    }

    callback_info = serials[serial] ;
    func = callback_info[0] ;
    ob = callback_info[1] ;

    if(!objectp(ob)) {
        _log(2, "Invalid object for callback") ;
        return ;
    }

    if(!valid_function(func)) {
        if(!function_exists(func, ob)) {
            _log(2, "Invalid function for callback") ;
            return ;
        }
    }

    _log(3, "Callback found. Executing %O on %O", func, ob) ;
    err = catch {
        if(valid_function(func))
            (*func)(copy(server)) ;
        else
            call_other(ob, func, copy(server)) ;
    } ;
    _log(3, "Callback executed") ;
}
