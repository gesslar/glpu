/**
 * @file /adm/daemons/websocket_echo.c
 * @description Test websocket with echo.websocket.org
 *
 * @created 2024/07/05 - Gesslar
 * @last_modified 2024/07/05 - Gesslar
 *
 * @history
 * 2024/07/05 - Gesslar - Created
 */

#include <daemons.h>
#include <websocket.h>
#include <socket.h>
#include <socket_err.h>

inherit STD_WS_CLIENT ;

private nosave mapping handles = ([ ]) ;
private nosave string host = "localhost" ;
private nosave string path = null ;
private nosave string server_name = "WebSocket Echo Server" ;

protected void periodic_message(int fd) ;
protected void send_outgoing_message(int fd,  string message) ;
protected void close_down_session(int fd) ;

void setup() {
    set_log_level(1) ;
    set_log_prefix("\e<re1>\e<0202>(WSS)\e<res>") ;
    set_option("tls", 1) ;
}

// Start the Connection
varargs void startup() {
    ws_request("wss://echo.websocket.org") ;
}

void handle_connected(int fd, mapping server) {
    mapping curr = ([ ]) ;
    int closing_time = 20+random(20) ;

    curr["host"] = server["host"] ;
    curr["port"] = server["port"] ;

    _log(1, "Scheduling periodic messaging") ;
    curr["messaging"] = call_out_walltime((: periodic_message, fd :), 5) ;

    _log(1, "Scheduling close down in %d seconds", closing_time) ;
    curr["closing"] = call_out_walltime((: close_down_session, fd :), closing_time) ;

    handles[fd] = curr ;
}

void handle_shutdown(int fd, mapping server) {
    mapping curr ;
    int messaging, closing ;

    if(!handles[fd]) {
        _log(2, "No handle for fd %d", fd) ;
        return ;
    }

    messaging = handles[fd]["messaging"] ;
    if(!nullp(messaging)) {
        if(find_call_out(messaging) != -1) {
            _log(1, "Stopping automatic messaging") ;
            remove_call_out(messaging) ;
        }
    }

    closing = handles[fd]["closing"] ;
    if(!nullp(closing)) {
        if(find_call_out(closing) != -1) {
            _log(1, "Stopping automatic close down") ;
            remove_call_out(closing) ;
        }
    }

    curr = handles[fd] ;

    map_delete(handles, fd);
}

// Function to parse text frames
void handle_text_frame(int fd, mapping frame_info) {
    string payload ;
    mixed message ;

    if(!frame_info) {
        _log(2, "No frame info") ;
        return ;
    }

    if(!handles[fd]["messaging"])
        return ;

    if(!handles[fd]["last_message"])
        return ;

    payload = to_string(frame_info["payload"]);
    message = parse_body(payload, "auto") ;

    if(!stringp(message)) {
        _log(2, "Received non-string payload: %O", message) ;
        return ;
    }

    _log(1, "Received message: %O", message) ;

    if(message == handles[fd]["last_message"]) {
        _log(1, "==> Messages match! 🍻") ;
    } else {
        _log(1, "==> Messages do not match! 😭") ;
    }
}

void periodic_message(int fd) {
    int messaging = handles[fd]["messaging"] ;
    string message ;

    if(!handles[fd]) {
        _log(2, "No handle for fd %d", fd) ;
        return ;
    }

    if(find_call_out(messaging) != -1) {
        _log(1, "Canceling existing messaging schedule") ;
        remove_call_out(messaging) ;
    }

    message = "Hello, world - " + base64_encode(""+time()) ;

    _log(2, "Sending periodic message") ;
    send_outgoing_message(fd, message) ;

    handles[fd]["last_message"] = message ;

    _log(2, "Scheduling next message") ;
    handles[fd]["messaging"] = call_out_walltime((: periodic_message, fd :), 10+random(20)) ;
}

void send_outgoing_message(int fd, string message) {
    _log(1, "Sending message: %O", message) ;

    if(!send_message(fd, WS_TEXT_FRAME, message))
        _log(1, "Failed to send message") ;
}

void close_down_session(int fd) {
    _log(1, "Automatically closing down session") ;

    if(!send_message(fd, WS_CLOSE_FRAME, WS_CLOSE_NORMAL, "Self-initiated close down"))
        _log(1, "Failed to send close frame") ;
}
