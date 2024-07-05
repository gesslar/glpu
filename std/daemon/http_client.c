// This is a base HTTP client daemon that is intended to be inherited by other
// objects to create a custom HTTP client. It provides the ability to send HTTP
// requests to a server and receive responses.
//
// Created:     2024/07/01: Gesslar
// Last Change: 2024/07/01: Gesslar
//
// 2024/07/01: Gesslar - Created

#include <socket.h>
#include <socket_err.h>
#include <http.h>
#include <daemons.h>

inherit STD_DAEMON ;
inherit M_HTTP ;

// Prototypes
varargs void http_connect(mapping request) ;
void process_redirect(int fd, mapping server) ;
void process_response(int fd, mapping server) ;
mixed send_http_request(int fd, mapping server) ;
protected nomask void shutdown_socket(int fd) ;

// Variables
private nosave mapping servers = ([ ]) ;
private nosave mapping resolve_keys = ([ ]) ;
private nosave int max_redirects = 5 ;
private nosave mapping default_headers ;

void mudlib_setup() {
    set_log_level(2) ;
    set_log_prefix("\e<re1>\e<0025>(CLIENT)\e<res>") ;
    default_headers = ([
        "User-Agent" : "FluffOS",
        "Connection" : "close",
        "Accept-Charset" : "utf-8",
    ]) ;

    set_option("cache", "/tmp/http/") ;
    // set_option("deflate", 1) ;
}

void post_setup() {
    string cache = get_option("cache") ;

    assure_dir(cache) ;

    if(get_option("deflate"))
        default_headers["Accept-Encoding"] = "deflate" ;
    else
        default_headers["Accept-Encoding"] = "identify" ;

    set_option("tls", 0) ;
}

varargs nomask void http_request(string url, string method, mapping headers, string body) {
    mapping parsed_url ;

    if(!stringp(url) || !stringp(method))
        error("Invalid URL, method, or headers specified.") ;

    parsed_url = parse_url(url) ;
    if(!parsed_url) {
        _log(0, "Failed to parse URL: %s", url) ;
        return ;
    }

    parsed_url["url"] = url ;
    parsed_url["method"] = method ;
    parsed_url["headers"] = headers ;
    parsed_url["body"] = body ;
    parsed_url["start_time"] = time_ns() ;

    http_connect(parsed_url) ;
}

varargs nomask void http_connect(mapping request) {
    int fd, status, key, port ;
    string host ;
    mapping server ;

    host = request["host"] ;

    fd = socket_create(get_option("tls") ? STREAM_TLS_BINARY : STREAM_BINARY, "socket_read", "socket_closed");

    if (fd < 0) {
        _log(0, "Unable to create socket: %s", socket_error(fd));
        return;
    }

    if(get_option("tls")) {
        socket_set_option(fd, SO_TLS_VERIFY_PEER, 1);
        socket_set_option(fd, SO_TLS_SNI_HOSTNAME, host);
    }

    server = ([
        "state" : HTTP_STATE_RESOLVING,
        "request" : request,
        "redirects" : 0,
    ]) ;

    _log(3, "Request: %O", server) ;

    servers[fd] = server ;

    key = resolve(host, "socket_resolve");
    resolve_keys[key] = fd ;
}

nomask void socket_resolve(string host, string addr, int key) {
    int fd, port, result ;
    mapping server ;

    fd = resolve_keys[key] ;
    map_delete(resolve_keys, key) ;

    server = servers[fd] ;
    if(!server)
        return ;

    // _log(2, "Server: %O", server) ;

    port = server["request"]["port"] ;

    if(addr) {
        _log(2, "Host resolved: %s (%s)", host, addr) ;

        server["ip"] = addr ;
        server["state"] = HTTP_STATE_CONNECTING ;
        server["host"] = host ;
        server["port"] = port ;
        servers[fd] = server ;

        if(function_exists("handle_connecting", this_object()))
            catch(call_other(this_object(), "handle_connecting", fd, server)) ;

        result = socket_connect(fd, addr + " " + port, "socket_read", "socket_ready");
        if(result != EESUCCESS) {
            _log(0, "Failed to connect to %s", host) ;

            server["state"] = HTTP_STATE_ERROR ;
            server["error"] = socket_error(result) ;
            servers[fd] = server ;

            if(function_exists("handle_connection_error", this_object()))
                call_other(this_object(), "handle_connection_error", fd, server) ;

            shutdown_socket(fd) ;
            return ;
        }

        server["state"] = HTTP_STATE_CONNECTED ;
        server["transactions"] = 0 ;
        server["cache"] = get_option("cache") + time_ns() ;
        server["received_body"] = 0 ;
        server["received_total"] = 0 ;
        servers[fd] = server ;
        _log(2, "Connected to %s %d", host, port) ;

        if(function_exists("handle_connected", this_object()))
            catch(call_other(this_object(), "handle_connected", fd, server)) ;
    } else {
        server["state"] = HTTP_STATE_ERROR ;
        server["error"] = "Failed to resolve hostname " + host ;
        servers[fd] = server ;

        if(function_exists("handle_resolve_error", this_object()))
            call_other(this_object(), "handle_resolve_error", fd, server) ;

        _log(0, "Failed to resolve hostname %s", host) ;

        shutdown_socket(fd) ;
        return ;
    }
}

nomask void socket_closed(int fd) {
    mapping server = servers[fd] ;
    float duration ;
    float speed ;

    if(!server)
        return ;

    _log(3, "Socket closed: %s %d", server["host"], server["port"]) ;

    if(function_exists("handle_closed", this_object()))
        catch(call_other(this_object(), "handle_closed", fd, server)) ;

    shutdown_socket(fd) ;
}

nomask void shutdown_socket(int fd) {
    mapping server = servers[fd] ;
    int result ;
    string cache_file ;
    float duration ;
    float speed ;
    float now, started ;
    float received_total ;

    if(!server)
        return ;

    cache_file = server["cache"] ;
    if(file_size(cache_file) > 0) {
        _log(2, "Total received: %d", server["received_total"]) ;
        _log(2, "Body received: %d", server["received_body"]) ;

        catch(process_response(fd, server)) ;
        // rm(cache_file) ;
    }

    now = to_float(time_ns()) ;
    started = to_float(server["request"]["start_time"]) ;

    duration = (now - started ) ;
    duration /= 1_000_000_000 ;

    received_total = to_float(server["received_total"]) ;
    speed = received_total / duration ;

    _log(3, "Transfer size: %d bytes", server["received_total"]) ;
    _log(3, "Transfer time: %.2f seconds", duration) ;
    _log(3, "Transfer speed: %.2f bytes/s", speed) ;

    if(function_exists("handle_shutdown", this_object()))
        catch(call_other(this_object(), "handle_shutdown", fd, server)) ;

    result = socket_close(fd) ;
    if(result != EESUCCESS) {
        _log(0, "Error closing socket: %s", socket_error(result)) ;
    } else {
        _log(4, "Socket removed: %s %d", server["host"], server["port"]) ;

    }

    map_delete(servers, fd) ;
}

nomask void socket_read(int fd, buffer incoming) {
    mapping server = servers[fd];
    int status_code ;
    buffer buf ;
    mapping http ;

    if (!server)
        return;

    _log(4, "First 10 bytes: %O", incoming[0..10]) ;

    server["transactions"] ++ ;

    _log(3, "================= STARTING TRANSACTION #%d =================", server["transactions"]) ;

    if(server["buffer"]) {
        buf = server["buffer"] + incoming ;

        map_delete(server, "buffer");
    } else {
        buf = incoming ;
    }

    server["received_total"] += sizeof(buf) ;
    http = server["http"] || ([]) ;
    if(sizeof(buf) && !http["status"]) {
        string status_string ;
        mapping status ;
        string *matches ;
        string str = to_string(buf) ;

        _log(4, "identify(str): %s", identify(str)) ;
        matches = pcre_extract(str, "^([[:print:][:space:]]+?)\r\n([[:print:][:space:]]+)") ;

        if(sizeof(matches) < 1) {
            server["buffer"] = buf ;
            servers[fd] = server ;
            return ;
        }
        status_string = matches[0] ;
        _log(3, "Status string: %s", status_string) ;
        status = parse_response_status(status_string) ;
        if(!status) {
            _log(4, "Failed to parse HTTP response status") ;
            server["state"] = HTTP_STATE_ERROR ;
            servers[fd] = server ;
            shutdown_socket(fd) ;
            return ;
        }

        buf = to_binary(matches[1]) ;
        http["status"] = status ;
        server["http"] = http ;
    }

    if(sizeof(buf) && !http["headers"]) {
        string header_string ;
        mapping headers ;
        string str = to_string(buf) ;
        string *matches ;

        matches = pcre_extract(str, "^([[:print:][:space:]]+?)\r\n\r\n([[:print:][:space:]]+)") ;
        _log(4, "matches 2: %O\n", matches) ;

        if(sizeof(matches) < 1) {
            server["buffer"] = buf ;
            servers[fd] = server ;
            return ;
        }

        header_string = matches[0] ;

        headers = parse_headers(header_string) ;
        if(!headers) {
            _log(0, "Failed to parse HTTP response headers") ;
            server["state"] = HTTP_STATE_ERROR ;
            servers[fd] = server ;
            shutdown_socket(fd) ;
            return ;
        }

        _log(3, "Received headers: %O", headers) ;

        if(sizeof(matches) == 2) {
            str = matches[1] ;
        }
        buf = to_binary(str) ;

        server["http"]["headers"] = headers ;
        servers[fd] = server ;
    }

    // Check for redirect status codes (301, 302, etc.)
    status_code = http["status"]["code"] ;

    if(member_array(status_code, HTTP_REDIRECT_CODES) != -1) {
        _log(1, "Redirecting") ;

        if(function_exists("handle_redirect", this_object()))
            catch(call_other(this_object(), "handle_redirect", fd, server));

        server["buffer"] = buf ;
        servers[fd] = server ;
        process_redirect(fd, server);
        return ;
    }
    server["received_body"] += sizeof(buf) ;
    cache_response(server["cache"], buf) ;
    // process_response(server) ;
// _log(1, "headers: %O", server["http"]["headers"]) ;
    return ;
}

nomask void socket_ready(int fd) {
    mapping server = servers[fd] ;
    mixed result ;

    if(!server)
        return ;

    server["state"] = HTTP_STATE_READY ;
    servers[fd] = server ;

    _log(2, "Ready to send request") ;

    if(function_exists("handle_ready", this_object()))
        catch(call_other(this_object(), "handle_ready", fd, server)) ;

    server["state"] = HTTP_STATE_SENDING ;
    servers[fd] = server ;

    if(function_exists("handle_sending", this_object()))
        catch(call_other(this_object(), "handle_sending", fd, server)) ;

    _log(2, "Sending request") ;
    _log(3, "Request: %O", server["request"]) ;
    result = send_http_request(fd, server) ;
    if(stringp(result)) {
        _log(0, "Failed to send request: %s", result) ;
        server["state"] = HTTP_STATE_ERROR ;
        servers[fd] = server ;
        shutdown_socket(fd) ;
        return ;
    }
}

private nomask void process_response(int fd, mapping server) {
    string file = server["cache"] ;
    mixed response ;

    if(file_size(file) < 1) {
        _log(0, "Failed to read cache file: %s", file) ;
        rm(file) ;
        return ;
    }

    _log(2, "Processing response: %s", file) ;

    if(!server["http"]["response"])
        server["http"]["response"] = ([ ]) ;

    // Chunked transfer encoding
    if(server["http"]["headers"]["transfer-encoding"] == "chunked") {
        string buf = "" ;
        mixed *assoc ;
        string *parts ;
        int *indices ;
        int i, sz ;
        int end_found ;
        string incoming, str ;

        if(!response)
            response = "" ;

        buf = read_cache(file) ;

        // assoc = pcre_assoc(buf, ({ "(\r\n0\r\n\r\n)", "(\r\n[0-9a-fA-F]+\r\n)", }), ({ 2, 1, })) ;
        assoc = pcre_assoc(buf, ({ "(\r\n0\r\n\r\n)", "(\r\n[0-9a-fA-F]+\r\n)", "([0-9a-fA-F]+\r\n)", "(\r\n)" }), ({ 2, 1, 1, 1 }));

        parts = assoc[0] ;
        indices = assoc[1] ;

        for(i = 0, sz = sizeof(parts); i < sz; i++) {
            if(indices[i] != 0) {
                if(indices[i] == 2)
                    end_found = 1 ;


                parts[i] = "" ;
            }
        }

        buf = implode(parts, "") ;

        response = buf ;
        server["http"]["response"]["body"] = response ;
        servers[fd] = server ;
        if(end_found) {
            _log(1, "Chunked transfer encoding complete") ;
            if(function_exists("handle_response", this_object()))
                catch(call_other(this_object(), "handle_response", server)) ;
        }
    } else if(server["http"]["headers"]["content-length"]) {
        int expected = server["http"]["headers"]["content-length"] ;
        response = read_cache(file) ;
        server["http"]["response"]["body"] = response ;
        servers[fd] = server ;
        if(sizeof(response) == expected) {
            _log(2, "Content-Length: %d", expected) ;
            _log(2, "Size of response: %d", sizeof(response)) ;
            if(function_exists("handle_response", this_object()))
                catch(call_other(this_object(), "handle_response", server)) ;
        }
    }

    _log(2, "Removing cache file: %s", file) ;
    rm(file) ;
}

nomask void process_redirect(int fd, mapping server) {
    mapping processed_url ;
    string location ;
    mapping headers ;

    _log(2, "Processing redirect") ;

    location = server["http"]["headers"]["location"];
    if(!stringp(location) || !strlen(location)) {
        _log(0, "No location header found in redirect response");
        return;
    }

    processed_url = parse_url(location);

    if(!processed_url) {
        _log(0, "Failed to parse URL: %s", location);
        shutdown_socket(fd);
        return;
    }

    shutdown_socket(fd);

    _log(1, "Redirecting to: %s", location);

    headers = server["headers"];
    if(!headers)
        headers = ([]) ;

    headers["X-Redirect-Count"] = server["redirects"] + 1 ;

    http_request(
        location,
        server["request"]["method"],
        headers,
        server["request"]["body"],
    );
}

nomask mixed send_http_request(int fd, mapping server) {
    string request, body ;
    mapping headers = server["request"]["headers"] ;
    string method = server["request"]["method"] ;
    string path = server["request"]["path"] ;
    string host = server["request"]["host"] ;
    int result ;

    _log(3, "Sending request: %s %s %s", method, path, host) ;

    if(!servers[fd])
        return "No such connection: " + fd ;

    if(!stringp(method) || !stringp(path) || !stringp(host)) {
        _log(2, "Invalid request parameters") ;
        return "Invalid request parameters" ;
    }
    method = upper_case(method);

    request = method + " " + path + " HTTP/1.1\r\n" ;
    request += "Host: " + host + "\r\n" ;

    if(!mapp(headers))
        headers = ([ ]) ;

    headers += default_headers ;

    foreach(string key, string value in headers) {
        request += key + ": " + value + "\r\n" ;
    }

    if(body) {
        request += "Content-Length: " + strlen(body) + "\r\n" ;
        if(headers["Content-Type"]) {
            string content_type = headers["Content-Type"] ;
            string *matches, type, subtype, charset ;

            matches = pcre_extract(
                content_type,
                "\\b(\\w+)/(\\w+)(?:\\s*;\\s*charset=(\\S+))?\\b"
            ) ;

            switch(sizeof(matches)) {
                case 2:
                    type = matches[0] ;
                    subtype = matches[1] ;
                    charset = "utf-8" ;
                    break ;
                case 3:
                    type = matches[0] ;
                    subtype = matches[1] ;
                    charset = matches[2] ;
                    break ;
                default:
                    _log(0, "Failed to parse Content-Type header: %s", content_type) ;
                    shutdown_socket(fd) ;
                    return "Failed to parse Content-Type header: " + content_type ;
                    break ;
            }

            request += sprintf("Content-Type: %s/%s; charset=%s\r\n", type, subtype, charset) ;
        }
    }

    request += "\r\n" ;

    if(body)
        request += body ;

    result = socket_write(fd, request) ;

    if(result != EESUCCESS) {
        _log(3, "Failed to send request: %s", socket_error(result)) ;
        shutdown_socket(fd) ;
        return "Failed to send request: " + socket_error(result) ;
    }

    _log(3, "Request sent: %s", identify(request)) ;

    return result ;
}

nomask int set_max_redirects(int max) {
    max_redirects = max ;
    return max_redirects ;
}

nomask int query_max_redirects() {
    return max_redirects ;
}
