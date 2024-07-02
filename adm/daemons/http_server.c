// /adm/daemons/http_server.c
// This is an HTTP server that may be inherited by other objects to create a
// custom HTTP server. It listens on a specified port and accepts incoming
// connections.
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
inherit M_LOG ;
inherit DM_HTTP ;

// Prototypes
varargs void send_http_response(int fd, string status, string content_type, string body) ;
void close_client_connection(int fd) ;
void close_all_client_connections() ;
void socket_shutdown(int fd) ;

// Variables
private nosave int LISTEN_PORT = 8080 ;
private nosave mapping clients = ([ ]) ;

int set_listen_port(int port) {
    if(port < 1 || port > 65535)
        return 0 ;

    LISTEN_PORT = port ;
    return LISTEN_PORT ;
}

int query_listen_port() {
    return LISTEN_PORT ;
}

void start_server() {
    int fd, status ;

    set_log_level(5) ;

    fd = socket_create(STREAM_BINARY, "socket_read", "socket_close");
    if (fd < 0) {
        _log(0, "Unable to create socket: %s", socket_error(fd));
        return;
    }

    status = socket_bind(fd, LISTEN_PORT);
    if (status < 0) {
        _log(0, "Unable to bind to port %d: %s", LISTEN_PORT, socket_error(status));
        return;
    }

    status = socket_listen(fd, "socket_listen");
    if (status < 0) {
        _log(0, "Unable to listen on socket: %s", socket_error(status));
        return;
    }

    _log(0, "Listening on port %d", LISTEN_PORT);
}

void socket_listen(int fd) {
    int client_fd ;
    mapping client ;
    string client_address ;
    string client_host ;
    int client_port ;

    client_fd = socket_accept(fd, "socket_read", "socket_write");
    if(client_fd < 0) {
        _log(0, "Error accepting client: %s", socket_error(client_fd));
        return;
    }

    client_address = socket_address(client_fd);
    sscanf(client_address, "%s %d", client_host, client_port);

    client = ([
        "host": client_host,
        "port": client_port,
        "time": time_ns(),
    ]) ;

    _log(1, "New connection from %s %d", client_host, client_port) ;

    clients[client_fd] = client ;
}

void close_all_client_connections() {
    foreach(int fd, mapping client in clients) {
        close_client_connection(fd) ;
    }
}

void close_client_connection(int fd) {
    mixed result ;

    socket_shutdown(fd) ;

    catch(result = socket_close(fd)) ;
    if(result < 0) {
        _log(0, "Error closing socket: %s", socket_error(result));
    }
}

void socket_shutdown(int fd) {
    mapping client = clients[fd] ;

    if(!client)
        return ;

    _log(0, "Removing socket: %s %d", client["host"], client["port"]);

    if(function_exists("handle_shutdown", this_object())) {
        catch(call_other(this_object(), "handle_shutdown", client)) ;
    }

    map_delete(clients, fd) ;
}

void socket_read(int fd, buffer incoming) {
    mixed err ;
    mapping result ;
    mapping client ;

    if(!bufferp(incoming))
        return ;

    client = clients[fd] ;
    if(!client) {
        _log(0, "Unknown client: %d", fd) ;
        return ;
    }

    _log(2, "Received: %O", to_string(incoming)) ;

    err = catch(result = parse_http(to_string(incoming))) ;
    _log(2, "Result: %O", result) ;
    if(err || !mapp(result)) {
        _log(2, "Error parsing: %O", err) ;
        _log(call_trace()) ;
        send_http_response(fd, HTTP_STATUS_INTERNAL_SERVER_ERROR, CONTENT_TYPE_TEXT_PLAIN) ;
        return ;
    }

    _log(2, "Parsed: %O", result) ;

    if(result == 0) {
        send_http_response(fd, HTTP_STATUS_BAD_REQUEST, CONTENT_TYPE_TEXT_PLAIN) ;
        return ;
    }

    clients[fd]["request"] = result ;

    if(function_exists("handle_request", this_object())) {
        catch(call_other(this_object(), "handle_request", fd, client)) ;
    } else {
        send_http_response(fd, HTTP_STATUS_NOT_IMPLEMENTED, CONTENT_TYPE_TEXT_PLAIN) ;
    }
}

varargs void send_http_response(int fd, string status, string content_type, string body) {
    string response;

    if(!fd || !status || !content_type)
        return ;

    if (!body) {
        switch(status) {
            case HTTP_STATUS_OK:
            case HTTP_STATUS_CREATED:
            case HTTP_STATUS_ACCEPTED:
            case HTTP_STATUS_NO_CONTENT:
            case HTTP_STATUS_MOVED_PERMANENTLY:
            case HTTP_STATUS_FOUND:
            case HTTP_STATUS_NOT_MODIFIED:
            case HTTP_STATUS_BAD_REQUEST:
            case HTTP_STATUS_UNAUTHORIZED:
            case HTTP_STATUS_FORBIDDEN:
            case HTTP_STATUS_NOT_FOUND:
            case HTTP_STATUS_METHOD_NOT_ALLOWED:
            case HTTP_STATUS_INTERNAL_SERVER_ERROR:
            case HTTP_STATUS_NOT_IMPLEMENTED:
            case HTTP_STATUS_BAD_GATEWAY:
            case HTTP_STATUS_SERVICE_UNAVAILABLE:
                sscanf(status, "%*d %s", body) ;
                break;
            default: body = status; break;
        }
    }

    // Build the HTTP response
    response = sprintf("HTTP/1.1 %s\r\n", status);
    response += sprintf("Content-Length: %d\r\n", body ? strlen(body) : 0);
    response += sprintf("Content-Type: %s\r\n", content_type);
    response += "Connection: close\r\n"; // Ensure connection is closed
    response += "\r\n"; // End of headers
    response += body || "" ; // Optional body

    // Send the response
    socket_write(fd, response);

    // Properly close the connection
    socket_close(fd);
}
