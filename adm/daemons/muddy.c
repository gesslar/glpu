/**
 * @file /adm/daemons/muddy.c
 * @description WebSockets chat daemon
 *
 * @created 2024/07/14 - Gesslar
 * @last_modified 2024/07/14 - Gesslar
 *
 * @history
 * 2024/07/14 - Gesslar - Created
 */

#include <muddy.h>

inherit STD_WS_CLIENT ;

private nosave mapping muddy = mud_config("MUDDY") ;

private nomask void muddy_handle_hello(mapping payload) ;
private nomask void muddy_handle_dispatch(mapping payload) ;
private nomask varargs void muddy_send_heartbeat(int immediate) ;
private nomask void muddy_handle_heartbeat_ack(mapping payload) ;
private nomask varargs void muddy_send_identify() ;
private nomask void muddy_send_message(string channel, string message) ;

void setup() {
    set_log_level(4) ;
    set_log_prefix("MUDDY") ;
}

void startup() {
    _debug(repeat_string("\n", 20)) ;

    websocket_connect(muddy["host"]) ;
}

void websocket_handle_connected() {
    _log(1, "Connected to %s", server["host"]) ;

    server["muddy"] = ([
        "ready": 0,
        "heartbeat_interval": 0.0,
    ]) ;
}

void websocket_handle_text_frame(mapping payload) {
    if(mapp(payload)) {
        switch(payload["op"]) {
            case MUDDY_DISPATCH:
                muddy_handle_dispatch(payload) ;
                break ;
            case MUDDY_HELLO:
                muddy_handle_hello(payload) ;
                break ;
            case MUDDY_HEARTBEAT:
                muddy_send_heartbeat(1) ;
                break ;
            case MUDDY_HEARTBEAT_ACK:
                muddy_handle_heartbeat_ack(payload) ;
                break ;
            default:
                _log(2, "Unknown opcode: %O", payload["opcode"]) ;
                break ;
        }
    } else {
        _log(2, "Invalid payload: %O", payload) ;
    }
}

void websocket_handle_message_sent(int opcode, mixed args...) {
    _log(2, "Message sent: %O", args) ;
}

private nomask void muddy_initial_heartbeat() {
    _log(2, "Sending initial heartbeat") ;

    muddy_send_heartbeat() ;
    server["muddy"]["heartbeat"] = null ;
}

varargs void muddy_send_heartbeat(int immediate) {
    int co, result ;
    string payload;

    _log(2, "Sending heartbeat") ;

    if(co = server["muddy"]["heartbeat"]) {
        if(find_call_out(co) != -1) {
            remove_call_out(co) ;
        }
    }

    payload = json_encode(([
        "op": MUDDY_HEARTBEAT,
    ]));

    result = websocket_message(WS_TEXT_FRAME, payload);

    if(!result) {
        _log(2, "Failed to send heartbeat");
    } else {
        _log(2, "Sent heartbeat to %s", server["request"]["host"]);
    }

    if(!immediate) {
        // Reschedule the next heartbeat
        server["muddy"]["heartbeat"] = call_out_walltime(
            "muddy_send_heartbeat",
            server["muddy"]["heartbeat_interval"]
        );
    }
}

private nomask void websocket_handle_ping_frame(mapping payload) {
    _log(1, "Received ping frame: %O", payload) ;
}

private nomask void muddy_handle_hello(mapping payload) {
    float heartbeat_interval ;

    _log(1, "Received hello message: %O", payload) ;

    heartbeat_interval = to_float(payload["d"]["heartbeat_interval"]) ;

    server["muddy"]["heartbeat_interval"] = heartbeat_interval ;

    muddy_send_identify() ;

    server["muddy"]["heartbeat"] = call_out_walltime("muddy_initial_heartbeat", random(100000) / 100000.0);
}

private nomask void muddy_send_identify() {
    string payload ;

    payload = json_encode(([
        "op": MUDDY_IDENTIFY,
        "d": ([
            "identifier" : get_config(__MUD_NAME__),
            "channels": muddy["channels"],
        ]),
    ])) ;

    websocket_message(WS_TEXT_FRAME, payload) ;
}

private nomask void muddy_handle_dispatch(mapping payload) {
    string event_name = payload["t"] ;
    mixed data = payload["d"] ;
    string event_handler ;


    event_handler = sprintf("muddy_handle_event_%s", event_name) ;
    event_handler = replace_string(event_handler, " ", "_") ;
    event_handler = lower_case(event_handler) ;

    _log(2, "Handling event %s in %s", event_name, event_handler) ;

    if(function_exists(event_handler)) {
        catch(call_other(this_object(), event_handler, data)) ;
    } else {
        _log(2, "No handler for event %s", event_name) ;
    }
}

private nomask void muddy_handle_heartbeat_ack(mapping payload) {
    _log(2, "Received Heartbeat ACK");

    if(!server["muddy"]["heartbeat"]) {
        _log(2, "Starting heartbeat with interval %.2fs", server["muddy"]["heartbeat_interval"]) ;
        server["muddy"]["heartbeat"] = call_out_walltime(
            "muddy_send_heartbeat",
            server["muddy"]["heartbeat_interval"]
        );
    }
}

nomask void muddy_handle_event_message_create(mapping data) {
    _log(1, "Received message: %O", data) ;

    CHAN_D->muddy_chat(data) ;
}

nomask void muddy_handle_event_echo(mapping data) {
    _log(1, "Received echo: %O", data) ;

    CHAN_D->muddy_chat(data) ;

}

public void muddy_send_message(string channel, string talker, string message) {
    string payload ;

    payload = json_encode(([
        "op": MUDDY_DISPATCH,
        "t": EVENT_MESSAGE_CREATE,
        "d": ([
            "channel": channel,
            "message": message,
            "talker": talker,
            "echo" : muddy["echo"],
        ]),
    ])) ;

    websocket_message(WS_TEXT_FRAME, payload) ;
}
