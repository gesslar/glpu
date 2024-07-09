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
void startup() ;
void http_callback(int fd, mapping response) ;

public nomask void start_bot(object bot) ;
private nomask void handle_event(int fd, mapping frame_info) ;
private nomask void handle_hello(int fd, mapping message) ;
private nomask void handle_ready(int fd, mapping message) ;
private nomask void send_identify(int fd) ;
private nomask varargs void send_heartbeat(int fd, int immediate) ;
private nomask void handle_heartbeat_ack(int fd, mapping message) ;
private nomask void initiate_discord(mapping response) ;
private nomask int bot_check(int fd) ;
private nomask object query_bot(int fd) ;
public nomask object query_bot_by_name(string name) ;

private nosave mapping handles = ([ ]) ;
private nosave mapping pending = ([ ]) ;

void setup() {
    set_no_clean(1) ;
    register_crash() ;

    set_log_level(1) ;
    set_log_prefix("(Discord)") ;

    handles = ([ ]);

    // call_out_walltime((:startup:), 0.5) ;
}

void start_bot(object bot, mixed *cb) {
    int id ;

    _log(1, "Connecting to Discord Gateway API for bot %s", bot->query_bot_name()) ;

    id = HTTPC_D->fetch(
        (: http_callback :),
        "https://discord.com/api/gateway",
        ([
            "Accept": "application/json",
            "Connection": "close",
        ])
    ) ;

    if(!id) {
        _log(1, "Failed to fetch Discord Gateway API") ;
    }

    pending[id] = ([
        "bot": bot,
        "callback" : cb,
    ]) ;
}

void stop_bot(int fd) {
    ws_close(fd, WS_CLOSE_NORMAL, "Bot stopped") ;
}

void http_callback(mapping incoming) {
    mapping status ;
    int code ;
    string mess ;
    mapping response, server ;
    int id ;
    object bot ;
    mixed *cb ;

    _log(2, "Received HTTP response") ;

    _log(4, "Entire HTTP conversation: %O", incoming) ;

    response = incoming["response"] ;
    status = response["status"] ;
    code = status["code"] ;
    mess = status["message"] ;
    id = incoming["request"]["start_time"] ;
    if(!objectp(pending[id]["bot"])) {
        _log(1, "No pending bot found.") ;
        return ;
    }
    response["pending"] = pending[id] ;
    map_delete(pending, id) ;

    if(code != 200) {
        _log(1, "Failed to connect to Discord Gateway API: %d - %s", code, mess) ;
        call_back(cb, ([ "fd": -1, "error": mess, ])) ;
        return ;
    }

    // Need to do this call_out, because there is some kind of race
    // condition with sockets closing before something crucial
    // happens. If you can figure out why, I'd be super grateful
    // for a fix.
    call_out_walltime((: initiate_discord, response :), 2.0) ;
    // initiate_discord(response) ;
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
    mixed err ;
    mapping handle ;
    mixed *cb ;

    if(!response) {
        _log(1, "No response from Discord Gateway API") ;
        return ;
    }

    _log(3, "body: %O", response["body"]) ;
    _log(3, "headers: %O", response["headers"]) ;
    _log(3, "bot: %O", response["bot"]) ;

    err = catch(payload = parse_body(response["body"], response["headers"]["content-type"])) ;
    if(err) {
        _log(1, "Failed to parse response body: %O", err) ;
        call_back(response["pending"]["callback"], ([ "fd": -1, "error": "Failed to parse response body" ])) ;
        // TODO: We can't cos we don't have the FD yet 🤷‍♂️
        shutdown_socket(fd) ;
        return ;
    }

    if(stringp(payload)) {
        _log(3, "Response body was not JSON") ;
        return ;
    }

    _log(3, "Payload: %O", payload) ;
    _log(2, "Sizeof payload: %d", sizeof(payload)) ;

    url = payload["url"] ;

    matches = pcre_extract(url, "^wss:\\/\\/(.*)\\/?$") ;
    if(!sizeof(matches)) {
        _log(1, "Failed to extract host and path from URL") ;
        return ;
    }

    host = matches[0] ;

    _log(2, "Discord Gateway Host found (%s)", matches[0]) ;
    _log(2, "Connecting to Discord Gateway (%s)", matches[0]) ;

    fd = ws_request(url) ;

    if(fd < 0) {
        _log(1, "Failed to connect to Discord Gateway %s", socket_error(fd)) ;
        call_back(response["pending"]["callback"], ([ "fd": -1, "error": socket_error(fd) ])) ;
        return ;
    }

    handle = ([
        "bot" : response["pending"]["bot"],
    ]) ;
    handles[fd] = handle ;

    call_back(response["pending"]["callback"], ([ "fd": fd ])) ;
}

void handle_connected(int fd, mapping server) {
    mapping handle ;
    object bot ;

    if(!bot_check(fd))
        return ;

    bot = query_bot(fd) ;
    _log(2, "%s Connected to Discord Gateway API", bot->query_bot_name()) ;

    _log(3, "Handle %O", handles[fd]) ;

    handle = handles[fd] ;
    handle["host"] = server["host"] ;
    handle["port"] = server["port"] ;
    handle["ready"] = 0 ;
    handle["heartbeat"] = null ;
    handle["heartbeat_interval"] = 0 ;
    handle["heartbeat_seq"] = 0 ;
    handles[fd] = handle ;

    if(function_exists("handle_connected", bot)) {
        _log(2, "Handing connected event to bot %s", bot->query_bot_name()) ;
        bot->handle_connected() ;
    }
}

void handle_shutdown(int fd, mapping server) {
    mapping handle ;
    int hb ;
    object bot ;

    _log(3, "Handling shutdown as requested")  ;

    if(!handles[fd]) {
        _log(2, "No handle for fd %d", fd) ;
        return ;
    }

    handle = handles[fd] ;

    if(!nullp(hb = handle["initial_heartbeat"])) {
        if(find_call_out(hb) != -1) {
            _log(2, "Stopping initial heartbeat") ;
            remove_call_out(hb) ;
        }
    }

    if(!nullp(hb = handle["heartbeat"])) {
        if(find_call_out(hb) != -1) {
            _log(2, "Stopping heartbeat") ;
            remove_call_out(hb) ;
        }
    }

    bot = handle["bot"] ;
    catch(bot->handle_shutdown()) ;

    map_delete(handles, fd);
}

void handle_close_frame(int fd, mixed payload) {
    mapping handle ;
    object bot ;

    if(!bot_check(fd))
        return ;

    bot = query_bot(fd) ;

    _log(2, "Received close frame") ;

    handle = handles[fd] ;
    if(handle["ready"]) {
        _log(2, "Bot %s has been disconnected", bot->query_bot_name()) ;
    } else {
        _log(2, "Bot %s failed to connect", bot->query_bot_name()) ;
    }
}

// Function to parse text frames
void handle_text_frame(int fd, mapping frame_info) {
    string payload ;
    mixed message ;
    object bot ;

    if(!bot_check(fd))
        return ;

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

void handle_event(int fd, mapping info) {
    mixed payload = json_decode(to_string(info["payload"])) ;
    string event_name = payload["t"] ;
    mixed data = payload["d"] ;
    object bot ;

    if(!bot_check(fd))
        return ;

    _log(2, "Received event: %s", event_name) ;

    bot = query_bot(fd) ;

    switch(event_name) {
        case "READY":
            // This event we handle here instead of in the bot only
            // because we need to record the session_id and resume_gateway_url
            // The bot can have it after we're done.
            handle_ready(fd, data) ;
            if(!bot_check(fd))
                return ;
            if(function_exists("handle_ready", bot)) {
                _log(2, "Handing %s event to bot %s", event_name, bot->query_bot_name()) ;
                bot->handle_ready(fd, data) ;
            }
            return ;
    }

    if(function_exists("handle_event", bot)) {
        _log(2, "Handing event %s to bot %s", event_name, bot->query_bot_name()) ;
        bot->handle_event(fd, event_name, data) ;
    } else {
        _log(3, "No event handler in bot %s", bot->query_bot_name()) ;
    }
}

void handle_hello(int fd, mapping payload) {
    int heartbeat_interval ;
    mapping handle ;

    if(!bot_check(fd))
        return ;

    heartbeat_interval = payload["d"]
    ["heartbeat_interval"];

    _log(2, "Received Hello with heartbeat interval (%d ms)", heartbeat_interval);
    handles[fd]["heartbeat_interval"] = heartbeat_interval / 1000.0;

    send_identify(fd);

    handles[fd]["initial_heartbeat"] = call_out_walltime("initial_heartbeat", random(100000) / 100000.0, fd) ;
}

void send_identify(int fd) {
    buffer frame ;
    int result ;
    string identify_payload ;
    mapping handle ;
    object bot ;
    int intents ;
    string token ;

    if(!bot_check(fd))
        return ;


    handle = handles[fd] ;
    bot = handle["bot"] ;

    _log(4, "Handle: %O", handle) ;
    if(!stringp(token = bot->query_token())) {
        _log(2, "No bot token for fd") ;
        ws_close(fd, WS_CLOSE_INTERNAL_SERVER_ERROR, "No bot token for fd") ;
        return ;
    }

    if(nullp(intents = bot->query_intents())) {
        _log(2, "No bot intents for fd") ;
        ws_close(fd, WS_CLOSE_INTERNAL_SERVER_ERROR, "No bot intents for fd") ;
        return ;
    }

    identify_payload = json_encode(([
        "op": 2,
        "d": ([
            "token": token,
            "intents": intents,
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

    if(!bot_check(fd))
        return ;


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

void initial_heartbeat(int fd) {
    if(!bot_check(fd))
        return ;

    _log(2, "Sending initial heartbeat") ;

    send_heartbeat(fd) ;
    handles[fd]["heartbeat"] = null ;
    map_delete(handles[fd], "initial_heartbeat") ;
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


// Process the ready frame
private nomask void handle_ready(int fd, mapping data) {
    mapping handle = handles[fd] ;
    mapping d ;
    mixed *matches ;
    string url ;

    if(!mapp(data)) {
        _log(2, "Received invalid ready frame");
        ws_close(fd, WS_CLOSE_INVALID_FRAME_PAYLOAD_DATA, "Invalid ready frame");
        return;
    }

    _log(2, "Received valid ready frame");

    handle["session_id"] = data["session_id"] ;

    url = data["resumed_gateway_url"] ;
    if(url) {
        matches = pcre_extract(url, "^wss:\\/\\/(.*)\\/?$") ;
        if(sizeof(matches)) {
            handle["resume_host"] = matches[0] ;
        }
    }

    handle["ready"] = 1 ;
    handles[fd] = handle ;

    _log(2, "Recorded session_id and resume_gateway_url") ;
}

private nomask int bot_check(int fd) {
    mapping handle = handles[fd] ;

    if(nullp(handle)) {
        // This shouldn't actually do anything, but just putting it here
        // for reasons.
        _log(2, "We lost our handle to the bot somehow???") ;
        ws_close(fd, WS_CLOSE_INTERNAL_SERVER_ERROR, "No connection.") ;
        return 0 ;
    }

    if(!objectp(query_bot(fd))) {
        _log(2, "Bot has vanished") ;
        ws_close(fd, WS_CLOSE_INTERNAL_SERVER_ERROR, "Bot has vanished") ;
        return 0 ;
    }

    return 1 ;
}

private nomask object query_bot(int fd) {
    mapping handle = handles[fd] ;

    _log(4, "Handle: %O", handle) ;
    if(nullp(handle))
        return null ;

    _log(4, "Bot: %O", handle["bot"]) ;
    if(!objectp(handle["bot"]))
        return null ;

    return handle["bot"] ;
}

public nomask object query_bot_by_name(string name) {
    int *fds = keys(handles) ;
    object bot ;

    foreach(int fd in fds) {
        bot = query_bot(fd) ;
        if(bot->query_bot_name() == name) {
            return bot ;
        }
    }

    return null ;
}

void event_on_remove(object prev) {
    int *fds = keys(handles) ;

    foreach(int fd in fds) {
        ws_close(fd, WS_CLOSE_GOING_AWAY, "Daemon removed") ;
    }
}

void crash() {
    event_on_remove(null) ;
}
