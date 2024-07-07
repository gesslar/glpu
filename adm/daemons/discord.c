/**
 * @file /adm/daemons/discord.c
 * @description Discord server daemon
 *
 * @created 2024/07/06 - Gesslar
 * @last_modified 2024/07/06 - Gesslar
 *
 * @history
 * 2024/07/06 - Gesslar - Created
 */

#include <daemons.h>
#include <discord.h>
#include <socket.h>
#include <socket_err.h>

inherit STD_WS_CLIENT ;

// This needs to receive the bot token.
void bot_token_retrieved(string token) ;
void http_callback(int fd, mapping response) ;

private nomask void startup() ;
void handle_hello(int fd, mapping message) ;
void send_identify(int fd) ;
varargs void send_heartbeat(int fd, int immediate) ;
void handle_heartbeat_ack(int fd, mapping message) ;
void initiate_discord(mapping response) ;

private nosave mapping handles = ([ ]) ;
private nosave string BOT_TOKEN ;

private nosave object http_client ;

void setup() {
    register_crash() ;

    set_no_clean(1) ;
    set_log_level(3) ;
    set_log_prefix("\e<re1>\e<0070>(Discord)\e<res>") ;

    handles = ([ ]);

    _log(1, "Setting up Discord Daemon") ;
    _log(1, "Fetching bot token") ;
    ENV_D->fetch("DISCORD_BOT_TOKEN", (: bot_token_retrieved :), "NONE", 1) ;
}

// Start connecting to Discord
varargs void startup() {
    string discord_api_host = "discord.com" ;
    string path = "api/gateway" ;
    mixed err;
    int fd ;
    mixed result ;

    if(!nullp(BOT_TOKEN)) {
        _log(1, "Bot token retrieved") ;
    } else {
        _log(1, "Bot token not retrieved") ;
        return ;
    }

    debug_message(repeat_string("\n", 20)) ;

    _log(1, "Connecting to Discord Gateway API") ;

    HTTPC_D->fetch(
        (: http_callback :),
        "https://discord.com/api/gateway",
        ([
            "Accept": "application/json",
            "Connection": "close",
        ])
    ) ;
}

void initiate_discord(mapping response) {
    int ret ;
    int fd ;
    int key ;
    string raw_handshake_key ;
    mixed payload ;
    string url ;
    string host, path ;
    string *matches ;

    if(!response) {
        _log(1, "No response from Discord Gateway API") ;
        return ;
    }

    _log(3, "body: %O", response["body"]) ;
    _log(3, "headers: %O", response["headers"]) ;

    payload = parse_body(response["body"], response["headers"]["content-type"]) ;

    if(stringp(payload)) {
        _log(3, "Response body was not JSON") ;
        return ;
    }

    _log(3, "Payload: %O", payload) ;

    url = payload["url"] ;

    matches = pcre_extract(url, "^wss:\\/\\/(.*)\\/?$") ;
    if(!sizeof(matches)) {
        _log(1, "Failed to extract host and path from URL") ;
        return ;
    }

    host = matches[0] ;

    _log(2, "Discord Gateway Host found (%s)", matches[0]) ;
    _log(2, "Connecting to Discord Gateway (%s)", matches[0]) ;

    ws_request(url) ;
}

void handle_connected(int fd, mapping server) {
    mapping handle = ([ ]) ;

    handle["host"] = server["host"] ;
    handle["port"] = server["port"] ;
    handle["ready"] = 0 ;
    handle["heartbeat"] = null ;
    handle["heartbeat_interval"] = 0 ;
    handle["heartbeat_seq"] = 0 ;

    handles[fd] = handle ;
}

void handle_shutdown(int fd, mapping server) {
    mapping handle ;
    int hb ;

    if(!mapp(handle = handles[fd])) {
        _log(3, "No handle for fd %d", fd) ;
        return ;
    }

    if(!nullp(hb = handle["heartbeat"])) {
        if(find_call_out(hb) != -1) {
            _log(2, "Stopping heartbeat") ;
            remove_call_out(hb) ;
        }
    }

    map_delete(handles, fd);
}

// TEXT FRAME PROCESSING
void handle_event(int fd, mapping frame_info) ;

// Function to parse text frames
void handle_text_frame(int fd, mapping frame_info) {
    string payload ;
    mixed message ;

    if(!frame_info) {
        _log(2, "No frame info") ;
        return ;
    }

    payload = to_string(frame_info["payload"]);
    if(payload[0] == '{' && payload[strlen(payload)-1] == '}') {
        message = json_decode(payload);
    } else {
        message = payload;
    }

    if (mapp(message)) {
        switch(message["op"]) {
            case DISCORD_DISPATCH:
                handle_event(fd, frame_info);
                break;
            case DISCORD_HEARTBEAT:
                send_heartbeat(fd, 1);
                break;
            case DISCORD_HELLO:
                handle_hello(fd, message);
                break;
            case DISCORD_HEARTBEAT_ACK:
                handle_heartbeat_ack(fd, message);
                break;
            default:
                _log(2, "Unknown text opcode %O", message["op"]) ;
        }
    } else {
        _log(2, "Received message: %s", message) ;
    }
}

// EVENT HANDLING
void handle_guild_create(int fd, mapping payload) ;
void handle_message_create(int fd, mapping payload) ;
void handle_ready(int fd, mapping payload) ;

void handle_event(int fd, mapping info) {
    mapping curr = handles[fd] ;
    mixed payload = json_decode(to_string(info["payload"])) ;
    string event_name = payload["t"] ;
    mixed data = payload["d"] ;

    _log(2, "Received event: %s", event_name) ;

    if(function_exists("handle_" + lower_case(event_name))) {
        call_other(this_object(), "handle_" + lower_case(event_name), fd, payload) ;
    } else {
        _log(3, "No handler for event: %s", event_name) ;
    }
}

void handle_guild_create(int fd, mapping payload) {
    _log(2, "Received GUILD_CREATE event") ;

    // Do something with the GUILD_CREATE event
}

void handle_message_create(int fd, mapping payload) {
    _log(2, "Received MESSAGE_CREATE event") ;

    // Do something with the MESSAGE_CREATE event
}

void handle_hello(int fd, mapping payload) {
    int heartbeat_interval = payload["d"]["heartbeat_interval"];

    _log(2, "Received Hello with heartbeat interval (%d ms)", heartbeat_interval);
    handles[fd]["heartbeat_interval"] = heartbeat_interval / 1000.0;

    send_identify(fd);
    call_out_walltime("initial_heartbeat", random(100000) / 100000.0, fd) ;
}

void initial_heartbeat(int fd) {
    _log(2, "Sending initial heartbeat") ;
    send_heartbeat(fd) ;
    handles[fd]["heartbeat"] = null ;
}

void handle_heartbeat_ack(int fd, mapping payload) {
    mapping curr = handles[fd] ;
    int seq ;

    _log(2, "Received Heartbeat ACK");

    seq = payload["d"] ;
    handles[fd]["heartbeat_seq"] = seq ;

    if(!curr["heartbeat"]) {
        _log(2, "Starting heartbeat");
        handles[fd]["heartbeat"] = call_out_walltime("send_heartbeat", handles[fd]["heartbeat_interval"], fd);
    }
}

void send_identify(int fd) {
    buffer frame ;
    int result ;
    string identify_payload ;

    identify_payload = json_encode(([
        "op": 2,
        "d": ([
            "token": BOT_TOKEN,
            "intents": GATEWAY_INTENT_BITS_DEFAULT |
                       GATEWAY_INTENT_BITS_MESSAGE_CONTENT,
            "properties": ([
                "os": "linux",
                "browser": get_config(__MUD_NAME__),
                "device": "FluffOS"
            ])
        ])
    ]));

    _log(2, "Sending Identify") ;

    result = send_message(fd, WS_TEXT_FRAME, identify_payload) ;

    if(!result) {
        _log(2, "Failed to send Identify payload");
    } else {
        _log(2, "Identify payload sent successfully");
    }
}

varargs void send_heartbeat(int fd, int immediate) {
    string payload;
    buffer frame;
    int result;
    int seq ;

    _log(2, "Sending heartbeat") ;
    if(!handles[fd]) {
        _log(2, "No handle for fd") ;
        return;
    }

    seq = handles[fd]["heartbeat_seq"] || 0 ;

    payload = json_encode(([
        "op": 1,
        "d": seq,
    ]));

    result = send_message(fd, WS_TEXT_FRAME, payload);
    if(!result) {
        _log(2, "Failed to send heartbeat");
    } else {
        _log(2, "Sent heartbeat to %s", handles[fd]["host"]) ;
    }

    if(!immediate) {
        // Reschedule the next heartbeat
        handles[fd]["heartbeat"] = call_out_walltime("send_heartbeat", handles[fd]["heartbeat_interval"], fd);
    }
}

// Process the ready frame
void handle_ready(int fd, mapping message) {
    mapping curr = handles[fd] ;
    mapping d ;
    mixed *matches ;
    string url ;

    if(!mapp(message)) {
        _log(2, "Received invalid ready frame");
        ws_close(fd, WS_CLOSE_INVALID_FRAME_PAYLOAD_DATA, "Invalid ready frame");
        return;
    }

    _log(2, "Received valid ready frame");

    d = message["d"] ;
    curr["session_id"] = d["session_id"] ;

    url = d["resumed_gateway_url"] ;
    if(url) {
        matches = pcre_extract(url, "^wss:\\/\\/(.*)\\/?$") ;
        if(sizeof(matches)) {
            curr["resume_host"] = matches[0] ;
        }
    }

    curr["ready"] = 1 ;
    handles[fd] = curr ;

    _log(2, "Recorded session_id and resume_gateway_url") ;
}

void bot_token_retrieved(string token) {
    if(token) {
        if(token == "NONE") {
            _log(1, "No bot token found") ;
            return ;
        }
        BOT_TOKEN = token ;
        startup() ;
    } else {
        _log(1, "Failed to retrieve bot token") ;
    }
}

void http_callback(mapping incoming) {
    mapping status ;
    int code ;
    string mess ;
    mapping response ;

    _log(2, "Received HTTP response") ;

    _log(3, "Entire HTTP conversation: %O", incoming) ;

    response = incoming["response"] ;
    status = response["status"] ;
    code = status["code"] ;
    mess = status["message"] ;

    if(code != 200) {
        _log(1, "Failed to connect to Discord Gateway API: %d - %s", code, mess) ;
        return ;
    }

    // Need to do this call_out, because there is some kind of race
    // condition with sockets closing before something crucial
    // happens. If you can figure out why, I'd be super grateful
    // for a fix.
    call_out_walltime((: initiate_discord, response :), 2.0) ;
    // initiate_discord(response) ;
}

void event_on_remove(object prev) {
    int *fds = keys(handles) ;

    foreach(int fd in fds) {
        ws_close(fd, WS_CLOSE_GOING_AWAY, "Daemon removed") ;
    }

    if(http_client) {
        http_client->remove() ;
    }
}

void crash() {
    event_on_remove(0) ;
}
