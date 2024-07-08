/**
 * @file /std/daemon/websocket_client.c
 * @description This is a base WebSocket client daemon that is intended to be
 *              inherited by other objects to create a custom WS client. It
 *              provides the ability to send start an HTTP session and then
 *              upgrade to a WebSocket connection to then send/receive
 *              messages.
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

inherit STD_DAEMON ;
inherit M_HTTP ;

// Functions
private nomask string random_string() ;
private nomask void ws_connect(mapping request) ;
protected nomask void shutdown_socket(int fd) ;
private nomask void process_handshake(int fd, mapping curr, buffer buf) ;
private nomask int is_message_complete(buffer buf) ;
private nomask mapping parse_websocket_frame(buffer buf) ;
private nomask void process_websocket_message(int fd, mapping frame_info) ;
private nomask buffer apply_mask(buffer data, buffer mask) ;
protected nomask void ws_close_socket(int fd) ;

protected nomask varargs int send_message(int fd, int frame_opcode, mixed args...) ;
protected nomask void send_pong(int fd, string payload) ;
protected nomask void send_ping(int fd);

// Variables
private nosave mapping servers, resolve_keys ;
private nosave string *subprotocols, *charset ;

void mudlib_setup() {
    set_no_clean(1) ;
    set_log_level(0) ;
    servers = ([ ]) ;
    resolve_keys = ([ ]) ;
    subprotocols = ({ }) ;

    charset = explode(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcde"
        "fghijklmnopqrstuvwxyz0123456789",
    "") ;
}

// Start connecting to Websocket
nomask void ws_request(string url) {
    string handshake_key ;
    mapping parsed_url ;

    if(!stringp(url)) {
        error("No url specified") ;
        return ;
    }

    parsed_url = parse_url(url) ;
    if(!parsed_url) {
        _log(2, "Failed to parse URL: %s", url) ;
        return ;
    }

    handshake_key = random_string(16); // Raw random string

    parsed_url["url"] = url ;
    parsed_url["start_time"] = time_ns() ;
    parsed_url["subprotocols"] = subprotocols ;
    parsed_url["handshake_key"] = handshake_key ;

    ws_connect(parsed_url) ;
}

void ws_socket_resolve(string host, string addr, int key) ;

private void ws_connect(mapping request) {
    int fd, key ;
    string host ;
    mapping server ;
    int secure ;

    host = request["host"] ;
    secure = request["secure"] ;

    fd = socket_create(secure ? STREAM_TLS_BINARY : STREAM_BINARY, "socket_read", "socket_closed");

    _log(4, "Socket created: %d", fd) ;

    if (fd < 0) {
        _log(0, "Unable to create socket: %s", socket_error(fd));
        return;
    }

    if(secure) {
        socket_set_option(fd, SO_TLS_VERIFY_PEER, 1);
        socket_set_option(fd, SO_TLS_SNI_HOSTNAME, host);
    }

    server = ([
        "state" : WS_STATE_RESOLVING,
        "request" : request,
    ]) ;

    _log(3, "Request: %O", request) ;

    servers[fd] = server ;

    key = resolve(host, (: ws_socket_resolve :)) ;
    resolve_keys[key] = fd ;
}

void ws_socket_resolve(string host, string addr, int key) {
    int fd, port, result ;
    mapping server ;

    if(nullp(resolve_keys[key])) {
        _log(0, "Failed to resolve key: %d", key) ;
        return ;
    }

    fd = resolve_keys[key] ;
    map_delete(resolve_keys, key) ;

    _log(4, "Socket resolved: %d", fd) ;

    server = servers[fd] ;
    if(!server)
        return ;

    port = server["request"]["port"] ;

    if(addr) {
        _log(2, "Host resolved: %s (%s)", host, addr) ;

        server["ip"] = addr ;
        server["state"] = WS_STATE_CONNECTING ;
        server["host"] = host ;
        server["port"] = port ;
        servers[fd] = server ;

        if(function_exists("handle_connecting", this_object()))
            catch(call_other(this_object(), "handle_connecting", fd, server)) ;

        result = socket_connect(fd, addr + " " + port, "socket_read", "socket_ready");
        if(result != EESUCCESS) {
            _log(0, "Failed to connect to %s", host) ;
            server["state"] = WS_STATE_ERROR ;
            server["error"] = socket_error(result) ;
            servers[fd] = server ;
            _log(1, "Error connecting to %s: %s", host, socket_error(result)) ;
            if(function_exists("handle_connection_error", this_object()))
                call_other(this_object(), "handle_connection_error", fd, server) ;

            shutdown_socket(fd) ;
            return ;
        }

        server["state"] = WS_STATE_HANDSHAKE ;
        server["transactions"] = 0 ;
        server["received_total"] = 0 ;
        servers[fd] = server ;
        _log(2, "Connected to %s %d", host, port) ;
    } else {
        server["state"] = WS_STATE_ERROR ;
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

    _log(3, "Socket closed for fd %d", fd) ;

    if(!server)
        return ;

    _log(3, "Socket closed: %s %d", server["host"], server["port"]) ;

    if(function_exists("handle_closed", this_object()))
        catch(call_other(this_object(), "handle_closed", fd, server)) ;

    shutdown_socket(fd) ;
}

protected nomask void shutdown_socket(int fd) {
    mapping server = servers[fd] ;
    int result ;
    float duration ;
    float now, started ;
    float received_total ;

    _log(3, "Shutting down socket for %d", fd) ;

    if(!server)
        return ;

    now = to_float(time_ns()) ;
    started = to_float(server["request"]["start_time"]) ;

    duration = (now - started ) ;
    duration /= 1_000_000_000 ;

    received_total = to_float(server["received_total"]) ;

    _log(3, "Total data sent: %d bytes", server["sent_total"]) ;
    _log(3, "Total transactions: %d", server["transactions"]) ;
    _log(3, "Total data received: %d bytes", server["received_total"]) ;
    _log(3, "Uptime: %.2f seconds", duration) ;

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

void socket_read(int fd, buffer incoming) {
    mapping server = servers[fd];
    buffer buf;
    mapping frame_info;
    mapping http;

    if (!server)
        return;

    server["transactions"]++;

    _log(3, "===========  STARTING WS TRANSACTION %d  ===========", server["transactions"]);

    if (server["buffer"]) {
        buf = server["buffer"] + incoming;
        map_delete(server, "buffer");
    } else {
        buf = incoming;
    }

    server["received_total"] += sizeof(buf);

    _log(3, "Incoming size: %d", sizeof(incoming));
    _log(3, "Buffer size: %d", sizeof(buf));

    // Process headers if not done yet
    server["response"] = server["response"] || ([]);

    if (sizeof(buf) && !server["response"]["status"]) {
        mapping status ;

        status = parse_response_status(buf, 1) ;
        if(!status) {
            server["buffer"] = buf;
            servers[fd] = server;
            return;
        }

        if(status["buffer"]) {
            buf = status["buffer"] ;
            map_delete(status, "buffer") ;
        }

        server["response"]["status"] = status ;

        _log(3, "Status found: %O", status) ;
    }

    if (sizeof(buf) && !server["response"]["headers"]) {
        mapping headers;

        headers = parse_headers(buf, 1);

        if (!headers) {
            server["buffer"] = buf;
            servers[fd] = server;
            return;
        }

        if(headers["buffer"]) {
            buf = headers["buffer"];
            map_delete(headers, "buffer");
        }

        server["response"]["headers"] = headers;

        _log(3, "Headers found: %O", headers);
    }

    if (server["state"] == WS_STATE_HANDSHAKE) {
        process_handshake(fd, server, buf);
        return;
    }

    if (server["state"] == WS_STATE_CONNECTED) {
        _log(2, "Processing WebSocket data");

        // Handle WebSocket data frames
        while (is_message_complete(buf)) {
            frame_info = parse_websocket_frame(buf);
            if (frame_info) {
                process_websocket_message(fd, frame_info);
                if (!servers[fd]) {
                    shutdown_socket(fd);
                    return;
                }
                buf = frame_info["buffer"];
            } else {
                break;
            }
        }

        // Save any remaining buffer data for next processing
        server["buffer"] = buf;
        servers[fd] = server;

        // Continue processing if there's still data left
        if (sizeof(buf))
            _log(2, "Data left in buffer for next transaction.");
    }

    _log(3, "Final buffer size: %d", sizeof(buf));
    _log(3, "===========  ENDING WS TRANSACTION %d  ===========", server["transactions"]);
}

private nomask void process_handshake(int fd, mapping server, buffer buf) {
    string raw_key = server["request"]["handshake_key"];
    string sec_websocket_key, concat, hash_result_hex, expected, accept;
    buffer hash_result_binary;
    mapping frame_info;
    mapping response = server["response"] ;

    _log(2, "Validating handshake");
    _log(4, "Server: %O", server);
    // _log(4, "Buffer: %O", buf);

    _log(2, "HTTP Status Code: %d", server["response"]["status"]["code"]);
    _log(2, "Header 'upgrade': %s", server["response"]["headers"]["upgrade"]);
    _log(2, "Header 'connection': %O", server["response"]["headers"]["connection"]);

    // Encode the raw key
    sec_websocket_key = base64_encode(to_binary(raw_key));
    _log(3, "Generated Key: %s", raw_key);

    // Concatenate base64 encoded key and WebSocket GUID
    concat = sec_websocket_key + WS_HANDSHAKE_GUID;

    // Compute SHA-1 hash
    hash_result_hex = hash("sha1", concat);

    // Convert hex to binary
    hash_result_binary = hex_to_binary(hash_result_hex);

    // Base64 encode the binary hash result
    expected = base64_encode(hash_result_binary);
    _log(3, "Base64 Encoded SHA-1 Hash (Expected): %s", expected);

    // Accept value from server
    accept = server["response"]["headers"]["sec-websocket-accept"];
    _log(3, "Header 'sec-websocket-accept': %s", accept);

    // Compare expected and accept values
    if (accept != expected) {
        _log(1, "Accept and Expected do not match.");
        server["state"] = WS_STATE_HANDSHAKE_FAILED;
        servers[fd] = server;
        shutdown_socket(fd);
        return;
    }

    _log(1, "Status[code]: %O", response["status"]["code"]);
    _log(1, "Headers[upgrade]: %O", response["headers"]["upgrade"]);
    _log(1, "Headers[connection]: %O", response["headers"]["connection"]);

    if (response["status"]["code"] != 101 ||
        response["headers"]["upgrade"] != "websocket" ||
        (member_array("Upgrade", response["headers"]["connection"]) == -1 &&
         member_array("upgrade", response["headers"]["connection"]) == -1) ||
        accept != expected) {
        _log(1, "Handshake invalid. Disconnecting.");
        server["state"] = WS_STATE_HANDSHAKE_FAILED;
        servers[fd] = server;
        shutdown_socket(fd);
        return;
    }

    _log(2, "Handshake validated");

    server["state"] = WS_STATE_CONNECTED;
    servers[fd] = server;

    if(function_exists("handle_connected", this_object()))
        catch(call_other(this_object(), "handle_connected", fd, server));

    _log(1, "Connected to %s", server["host"]);
}

// Function to check if the buffer contains a complete WebSocket frame
private nomask int is_message_complete(buffer buf) {
    int payload_length;
    int offset;

    if (sizeof(buf) < 2) {
        return 0;
    }

    payload_length = buf[1] & 0x7F;
    offset = 2;

    if (payload_length == 126) {
        if (sizeof(buf) < 4) {
            _log(3, "Not enough data to determine the full payload length") ;
            return 0; // Not enough data to determine the full payload length
        }
        payload_length = (buf[2] << 8) | buf[3];
        offset = 4;
    } else if (payload_length == 127) {
        if (sizeof(buf) < 10) {
            _log(3, "Not enough data to determine the full payload length") ;
            return 0; // Not enough data to determine the full payload length
        }
        payload_length = ((buf[2] & 0xFF) << 56) | ((buf[3] & 0xFF) << 48) |
                         ((buf[4] & 0xFF) << 40) | ((buf[5] & 0xFF) << 32) |
                         ((buf[6] & 0xFF) << 24) | ((buf[7] & 0xFF) << 16) |
                         ((buf[8] & 0xFF) << 8)  |  (buf[9] & 0xFF);
        offset = 10;
    }

    // Handle masked payloads
    if (buf[1] & 0x80) {
        offset += 4;
    }

    // Check if we have enough data for the complete frame
    if (sizeof(buf) < offset + payload_length) {
        _log(3, "Not enough data for the full payload") ;
        return 0;
    }

    _log(3, "Enough data for the full payload") ;

    return 1;
}

private nomask mapping parse_websocket_frame(buffer buf) {
    mapping result = ([]);
    int fin, opcode, masked, payload_length, offset;
    buffer payload;

    if (sizeof(buf) < 2) {
        _log(3, "Insufficient data to process");
        return 0;
    }

    fin = buf[0] & 0x80;
    opcode = buf[0] & 0x0F;
    masked = buf[1] & 0x80;
    payload_length = buf[1] & 0x7F;
    offset = 2;

    _log(3, "Initial frame details - fin: %d, opcode: %d, masked: %d, payload_length: %d", fin, opcode, masked, payload_length);

    if (payload_length == 126) {
        if (sizeof(buf) < 4) {
            _log(3, "Insufficient data for extended payload length (126)");
            return 0;
        }
        payload_length = (buf[2] << 8) | buf[3];
        offset = 4;
    } else if (payload_length == 127) {
        if (sizeof(buf) < 10) {
            _log(3, "Insufficient data for extended payload length (127)");
            return 0;
        }
        payload_length = ((buf[2] & 0xFF) << 56) | ((buf[3] & 0xFF) << 48) |
                         ((buf[4] & 0xFF) << 40) | ((buf[5] & 0xFF) << 32) |
                         ((buf[6] & 0xFF) << 24) | ((buf[7] & 0xFF) << 16) |
                         ((buf[8] & 0xFF) << 8) | (buf[9] & 0xFF);
        offset = 10;
    }

    _log(3, "Processed payload length: %d, offset: %d", payload_length, offset);

    if (masked)
        offset += 4;

    if (sizeof(buf) < offset + payload_length) {
        _log(3, "Insufficient data for full payload. Needed: %d, available: %d", offset + payload_length, sizeof(buf));
        return 0;
    }

    payload = allocate_buffer(payload_length);

    if (masked) {
        buffer mask = buf[2..5];
        for (int i = 0; i < payload_length; i++) {
            payload[i] = buf[offset + i] ^ mask[i % 4];
        }
    } else {
        payload = buf[offset..offset + payload_length - 1];
    }

    _log(3, "Payload unmasked or copied");

    result["fin"] = fin;
    result["opcode"] = opcode;
    result["masked"] = masked;
    result["payload_length"] = payload_length;
    result["payload"] = payload;

    // Update the buffer to only contain unprocessed data
    result["buffer"] = buf[offset + payload_length..];

    _log(3, "Offset: %d, Payload Length: %d, Buffer Size: %d", offset, payload_length, sizeof(buf));
    _log(3, "Remaining buffer size: %d", sizeof(result["buffer"]));

    return result;
}

// WEBSOCKET HANDLERS
private nomask void process_text_frame(int fd, mapping frame_info) ;
private nomask void process_close_frame(int fd, mapping frame_info) ;
private nomask void process_ping_frame(int fd, mapping frame_info) ;
private nomask void process_pong_frame(int fd, mapping frame_info) ;
private nomask void process_continuation_frame(int fd, mapping frame_info) ;
private nomask void process_binary_frame(int fd, mapping frame_info) ;
private nomask void process_unknown_frame(int fd, mapping frame_info) ;

private nomask void process_websocket_message(int fd, mapping frame_info) {
    mapping server;
    buffer payload;
    int opcode;

    server = servers[fd];
    payload = frame_info["payload"];
    opcode = frame_info["opcode"];

    // Ensure we have data to process
    if (!sizeof(payload)) {
        _log("No data to process");
        return;
    }

    // Process the frame based on the opcode
    if (opcode == WS_TEXT_FRAME) {
        _log(2, "Received text frame: %s", to_string(payload));
        process_text_frame(fd, frame_info);
    } else if (opcode == WS_CLOSE_FRAME) {
        _log(0, "\e[38;5;124mReceived close frame:\e[0m %O", binary_to_hex(payload));
        _log(2, "Received close frame: %O", binary_to_hex(payload));
        process_close_frame(fd, frame_info);
    } else if (opcode == WS_PING_FRAME) {
        _log(2, "Received ping frame");
        process_ping_frame(fd, frame_info);
    } else if (opcode == WS_PONG_FRAME) {
        _log(2, "Received pong frame");
        process_pong_frame(fd, frame_info);
    } else if (opcode == WS_CONTINUATION_FRAME) {
        _log(2, "Received continuation frame");
        process_continuation_frame(fd, frame_info);
    } else if (opcode == WS_BINARY_FRAME) {
        _log(2, "Received binary frame");
        process_binary_frame(fd, frame_info);
    } else {
        _log(2, "Received unknown frame with opcode %d", opcode);
    }

    if (!servers[fd])
        return;

    // Save any remaining buffer data for next processing
    server["buffer"] = frame_info["buffer"];
    servers[fd] = server;
}

private nomask void process_text_frame(int fd, mapping frame_info) {
    string payload = to_string(frame_info["payload"]);
    _log(3, "Payload: %s", payload);
    if(function_exists("handle_text_frame", this_object()))
        catch(call_other(this_object(), "handle_text_frame", fd, frame_info));
}

private nomask void process_close_frame(int fd, mapping frame_info) {
    int close_code = 0;

    string close_reason = "<no reason given>";
    if (sizeof(frame_info["payload"]) >= 2) {
        close_code = (frame_info["payload"][0] << 8) | frame_info["payload"][1];
        if (sizeof(frame_info["payload"]) > 2) {
            close_reason = to_string(frame_info["payload"][2..]);
        }
    }
    _log(0, "Connection closed by server. Code: %d, Reason: %s", close_code, close_reason);

    if (function_exists("handle_close_frame", this_object())) {
        catch(call_other(this_object(), "handle_close_frame", fd, frame_info));
    }
}

private nomask void process_ping_frame(int fd, mapping frame_info) {
    _log(2, "Received ping frame");
    send_pong(fd, to_string(frame_info["payload"]));

    if(function_exists("handle_ping_frame", this_object()))
        catch(call_other(this_object(), "handle_ping_frame", fd, frame_info));
}

private nomask void process_pong_frame(int fd, mapping frame_info) {
    _log(2, "Received pong frame");

    if(function_exists("handle_pong_frame", this_object()))
        catch(call_other(this_object(), "handle_pong_frame", fd, frame_info));
}

private nomask void process_continuation_frame(int fd, mapping frame_info) {
    _log(2, "Received continuation frame");

    if(function_exists("handle_continuation_frame", this_object()))
        catch(call_other(this_object(), "handle_continuation_frame", fd, frame_info));
}

private nomask void process_binary_frame(int fd, mapping frame_info) {
    _log(2, "Received binary frame");

    if(function_exists("handle_binary_frame", this_object()))
        catch(call_other(this_object(), "handle_binary_frame", fd, frame_info));
}

private nomask void process_unknown_frame(int fd, mapping frame_info) {
    _log(2, "Received unknown frame");

    if(function_exists("handle_unknown_frame", this_object()))
        catch(call_other(this_object(), "handle_unknown_frame", fd, frame_info));
}

private nomask varargs buffer format_frame(int opcode, mixed args...) {
    int len, i, frame_offset;
    buffer frame;
    buffer payload;
    int close_code;
    string reason;
    buffer close_payload;
    buffer mask;
    buffer masked_payload;

    if (!arrayp(args)) {
        args = ({ args });
    }

    // Determine the payload based on the opcode
    switch(opcode) {
        case WS_CLOSE_FRAME: {
            if (sizeof(args) < 1 || !intp(args[0])) {
                _log(2, "Invalid close code: %O", args[0]);
                return 0;
            }
            close_code = args[0];
            reason = (sizeof(args) > 1 && stringp(args[1])) ? args[1] : "";
            close_payload = allocate_buffer(2 + strlen(reason));
            close_payload[0] = (close_code >> 8) & 0xFF;
            close_payload[1] = close_code & 0xFF;
            close_payload[2..] = to_binary(reason);
            // payload = to_string(close_payload);
            payload = close_payload;
            break;
        }
        case WS_TEXT_FRAME:
        case WS_BINARY_FRAME:
        case WS_PING_FRAME:
        case WS_PONG_FRAME:
            payload = args[0];
            break;

        default:
            _log(3, "Unsupported frame opcode: %d", opcode);
            return 0;
    }

    len = sizeof(payload);
    mask = allocate_buffer(4);

    // Generate a random masking key
    for (i = 0; i < 4; i++) {
        mask[i] = random(256);
    }

    if (len <= 125) {
        frame = allocate_buffer(2 + len + 4); // 2 bytes header + 4 bytes mask key + payload
        frame[1] = 0x80 | len; // Mask bit set + payload length
        frame_offset = 2; // Header size
    } else if (len <= 65535) {
        frame = allocate_buffer(4 + len + 4); // 2 bytes header + 2 bytes extended payload length + 4 bytes mask key + payload
        frame[1] = 0x80 | 126; // Mask bit set + 126
        frame[2] = (len >> 8) & 0xFF;
        frame[3] = len & 0xFF;
        frame_offset = 4; // Header size + extended payload length
    } else {
        frame = allocate_buffer(10 + len + 4); // 2 bytes header + 8 bytes extended payload length + 4 bytes mask key + payload
        frame[1] = 0x80 | 127; // Mask bit set + 127
        for (i = 0; i < 8; i++) {
            frame[2 + i] = (len >> (8 * (7 - i))) & 0xFF;
        }
        frame_offset = 10; // Header size + extended payload length
    }

    frame[0] = 0x80 | opcode; // FIN bit set + opcode
    frame[frame_offset..(frame_offset + 3)] = mask; // Set the mask key

    masked_payload = apply_mask(payload, mask);
    frame[(frame_offset + 4)..] = masked_payload;

    return frame;
}

// Function to apply mask to a buffer
private nomask buffer apply_mask(buffer data, buffer mask) {
    int length = sizeof(data);
    buffer masked_data = allocate_buffer(length);

    for (int i = 0; i < length; i++) {
        masked_data[i] = data[i] ^ mask[i % 4];
    }

    return masked_data;
}

protected nomask varargs int send_message(int fd, int frame_opcode, mixed args...) {
    buffer frame ;
    int result ;
    int written ;

    if(!servers[fd])
        return;

    if(member_array(frame_opcode, WS_VALID_FRAME_OPCODES) == -1) {
        _log(3, "Invalid opcode: %d", frame_opcode);
        return 0;
    }

    if(nullp(args)) {
        _log(3, "No payload provided for frame opcode: %d", frame_opcode);
        return 0;
    }

    if(!pointerp(args))
        args = ({ args });

    frame = format_frame(frame_opcode, args...);
    written = sizeof(frame) ;
    servers[fd]["sent_total"] += written ;
    result = socket_write(fd, frame);

    _log(3, "Frame type: %d, Message: %O", frame_opcode, args);
    _log(3, "Binary frame: %O", binary_to_hex(frame));

    if(result != EESUCCESS) {
        _log(2, "Failed to send message: %s", socket_error(result)) ;
        shutdown_socket(fd) ;
        return 0 ;
    } else {
        _log(2, "Sent message to %s", servers[fd]["host"]) ;
        return 1 ;
    }
}

// Send a ping frame
protected nomask void send_ping(int fd) {
    mapping server = servers[fd];

    _log(2, "Sending ping frame") ;

    if (server && server["state"] == WS_STATE_CONNECTED) {
        int result ;

        result = send_message(fd, WS_PING_FRAME, "");
        if(!result) {
            _log(2, "Failed to send ping: %d", result) ;
        } else {
            _log(2, "Sent ping to ", server["host"]) ;
        }
    }
}

protected nomask void send_pong(int fd, string payload) {
    int result ;
    mapping server = servers[fd] ;

    _log(2, "Sending pong frame");

    if(!server) {
        _log(0, "No server data for fd %d", fd) ;
        return ;
    }

    result = send_message(fd, WS_PONG_FRAME, payload);
    if(!result) {
        _log(2, "Failed to send pong: %d", result) ;
    } else {
        _log(2, "Sent pong to %s", server["host"]) ;
    }
}

void socket_ready(int fd) {
    string out;
    string raw_key, sec_websocket_key;
    string host ;
    int result ;
    string subs ;
    mapping server ;
    int written ;

    _log(2, "Socket fully connected, now able to send data");

    server = servers[fd] ;

    if(!server) {
        _log(0, "No server data for fd %d", fd) ;
        return ;
    }

    server["state"] = WS_STATE_HANDSHAKE ;

    if(!stringp(raw_key = server["request"]["handshake_key"])) {
        _log(0, "No handshake key for fd %d", fd);
        shutdown_socket(fd);
        return;
    }

    if(!server["host"]) {
        _log(0, "No host specified for fd %d", fd);
        shutdown_socket(fd);
        return;
    }

    sec_websocket_key = base64_encode(to_binary(raw_key));
    _log(2, "Sec-WebSocket-Key Sent to %s: %s", sec_websocket_key, server["host"]);

    out = sprintf(
        "GET / HTTP/1.1\r\n"
        "Host: "+server["host"]+"\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: %s\r\n"
        "Sec-WebSocket-Version: 13\r\n",
        sec_websocket_key
    );

    if(sizeof(server["subprotocols"])) {
        subs = implode(server["subprotocols"], ", ");
        subs = sprintf("Sec-WebSocket-Protocol: %s\r\n", subs);
    }

    if(strlen(subs))
        out += subs;

    out += "\r\n" ;

    _log(2, "Sending handshake") ;
    _log(3, "Handshake request: %s", identify(out)) ;

    written = sizeof(to_binary(out)) ;
    server["sent_total"] = written ;
    result = socket_write(fd, out);
    if(result != EESUCCESS) {
        _log(2, "Failed to send handshake request: %d", result);
        shutdown_socket(fd);
    } else {
        _log(2, "Handshake request sent");
    }

    servers[fd] = server ;
}

void ws_close(int fd, int code, string reason) {
    send_message(fd, WS_CLOSE_FRAME, code, reason) ;
    shutdown_socket(fd) ;
}

void ws_close_socket(int fd) {
    mixed *sstatus ;
    int err ;

    sstatus = socket_status(fd);
    if(sstatus[1] != WS_STATE_CLOSED) {
        _log(3, "Closing server socket.") ;
        err = socket_close(fd);
        if(err != EESUCCESS) {
            _log(3, "Error closing socket: %s", socket_error(err));
        }
    }

    if(servers[fd]) {
        _log(0, "Cleaning up websocket data");
        shutdown_socket(fd);
    }
}

protected nomask string random_string(int length) {
    string result = "";
    while(length--)
        result += element_of(charset);
    return result;
}

void event_on_remove(object prev) {
    foreach(int fd, mapping server in servers) {
        catch(shutdown_socket(fd)) ;
    }
}
