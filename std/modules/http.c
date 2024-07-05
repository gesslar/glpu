// /adm/daemons/modules/http.c
// Modules for HTTP servers/clients
// Created:     2024/07/01: Gesslar
// Last Change: 2024/07/01: Gesslar
//
// 2024/07/01: Gesslar - Created

inherit M_LOG ;

#include <daemons.h>
#include <localtime.h>
#include <type.h>
#include <http.h>

string url_encode(string str) ;
string url_decode(string str) ;

buffer to_binary(string str) ;
string to_string(buffer buf) ;

varargs int find_marker(mixed buf, string marker) ;
varargs int bufsrch(buffer buf, mixed str) ;
void cache_response(string file, mixed response) ;
mixed read_cache(string file) ;
protected nomask void set_option(string key, mixed value) ;
protected nomask mixed get_option(string key) ;

mapping parse_http(string str) ;
mapping parse_http_response(string str) ;
mapping parse_http_request(string str) ;
mapping parse_headers(string str) ;
mapping parse_body(string str, string content_type) ;
mapping parse_route(string str) ;
mapping parse_query(string str) ;
mapping parse_url(string str) ;

private nomask nosave mapping options = ([]) ;

mapping parse_http(string str) {
    string request, header, body ;
    mapping result = ([]) ;
    int start_cursor, end_cursor ;
    string content_type ;

    end_cursor = find_marker(str, "\r\n\r\n") ;

    if(end_cursor == -1)
        return 0 ;

    header = str[0..end_cursor-1] ;
    start_cursor = find_marker(header, "\r\n") ;
    request = header[0..start_cursor-1] ;
    body = str[end_cursor+4..] ;

    result["request"] = parse_http_request(request) ;
    if(!result["request"]) return 0 ;
    result["route"] = parse_route(result["request"]["path"]) ;
    if(!result["route"]) return 0 ;
    result["headers"] = parse_headers(header) ;
    if(!result["headers"]) return 0 ;

    content_type = result["headers"]["content-type"];
    result["body"] = parse_body(body, content_type);

    return result ;
}

mapping parse_http_response(string str) {
    string header, body ;
    mapping result = ([]) ;
    int start_cursor, end_cursor ;
    string test ;
    int i, sz ;
    string *lines ;

    start_cursor = 0 ;
    end_cursor = find_marker(str, "\r\n\r\n") ;
    if(end_cursor == -1)
        return 0 ;

    header = str[0..end_cursor-2] ;
    body = str[end_cursor+2..] ;

    _log(1, "header: %s", header) ;

    result["headers"] = parse_headers(header) ;
    if(!result["headers"]) return 0 ;

    result["body"] = parse_body(body, result["headers"]["content-type"]) ;
    if(!result["body"]) return 0 ;

    return result ;
}

mapping parse_http_request(string request_line) {
    string *parts;
    mapping result = ([]);

    // Define the regex pattern for matching the HTTP method, path, and version
    string pattern = "^(GET|POST|PUT|DELETE|OPTIONS|HEAD|PATCH|TRACE|CONNECT)\\s+([^\\s]+)\\s+HTTP/(\\d\\.\\d)$";

    // Extract the method, path, and version using regex
    parts = pcre_extract(request_line, pattern);

    // Ensure parts were extracted correctly
    if (sizeof(parts) == 3) {
        result["method"] = parts[0];
        result["path"] = parts[1];
        result["version"] = parts[2];
    } else {
        return 0;
    }

    return result;
}

mapping parse_route(string route) {
    mapping result = ([]) ;
    string *parts;

    // Define the regex pattern for matching the path and optional query
    string pattern = "^(/[^?]*)(?:\\?(.*))?$";

    // Extract the path and query using regex
    parts = pcre_extract(route, pattern);

    // Ensure parts were extracted correctly
    if (sizeof(parts) >= 1) { // parts[0] is the path, parts[1] is the query if present
        result["route"] = parts[0];
        if (sizeof(parts) == 2) {
            mapping query = parse_query(parts[1]);
            result["query"] = query;
        } else {
            result["query"] = "";
        }
    } else {
        throw("Bad query.");
    }

    return result;
}

mapping parse_response_status(string str) {
    string *parts;
    mapping result = ([]);

    // Define the regex pattern for matching the HTTP status code and message
    string pattern = "^HTTP/\\d\\.\\d\\s+(\\d{3})\\s+(.*)$";

    // Extract the status code and message using regex
    parts = pcre_extract(str, pattern);

    // Ensure parts were extracted correctly
    if (sizeof(parts) == 2) {
        result["code"] = to_int(parts[0]);
        result["message"] = parts[1];
    } else {
        return 0;
    }

    return result;

}

mapping parse_headers(string str) {
    mapping headers = ([]);
    string* header_lines;
    string header_line;
    string header_name;
    string header_value;
    int i;

    header_lines = explode(str, "\r\n");
    for (i = 0; i < sizeof(header_lines); i++) {
        header_line = header_lines[i];

        if (header_line == "") {
            break;
        }

        if(sscanf(header_line, "%s: %s", header_name, header_value) == 2) {
            mixed t ;

            // per HTTP 1.1 spec, header names are case-insensitive
            header_name = lower_case(header_name);

            if(sscanf(header_value, "%d", t)) {
                headers[header_name] = t ;
            } else if(sscanf(header_value, "%f", t)) {
                headers[header_name] = t ;
            } else {
                // Do known array-based headers
                switch(header_name) {
                    case "connection":
                    case "accept-encoding":
                        headers[header_name] = explode(header_value, ",") ;
                        headers[header_name] = map(headers[header_name], (: trim :)) ;
                        break ;
                    default:
                        headers[header_name] = header_value ;
                }
            }
        }
    }

    return headers;
}

mapping parse_query(string str) {
    mapping payload = ([]) ;
    string *args ;

    args = explode(str, "&") ;

    if(sizeof(args)) {
        foreach(string arg in args) {
            string *parts ;

            parts = explode(arg, "=") ;
            if(sizeof(parts) == 2) {
                payload[url_decode(parts[0])] = url_decode(parts[1]) ;
            }
        }
    }

    return payload ;
}

mixed parse_body(string body, string content_type) {
    mixed payload = ([]);

    if (!body || body == "")
        return 0;

    switch (content_type) {
        case CONTENT_TYPE_APPLICATION_JSON:
            // Attempt to decode JSON payload
            if (body[0] == '{' && body[<1] == '}') {
                catch(payload = json_decode(body));
            }
            break;

        case CONTENT_TYPE_APPLICATION_FORM_URLENCODED: {
            // Decode URL-encoded form data
            string *args;
            args = explode(body, "&");
            if (sizeof(args)) {
                payload = ([]);
                foreach (string arg in args) {
                    string *parts;
                    parts = explode(arg, "=");
                    if (sizeof(parts) == 2) {
                        payload[url_decode(parts[0])] = url_decode(parts[1]);
                    }
                }
            }
            break;
        }
        case "auto" :
            // Attempt to decode JSON payload
            if(body[0] == '{' && body[<1] == '}')
                return parse_body(body, "application/json") ;
            else
                return parse_body(body, CONTENT_TYPE_TEXT_PLAIN) ;
            break ;
        default:
            // Just send it back
            payload = body;
            break;
    }

    return payload;
}

string url_encode(string str) {
    string allowed = "-_.~";
    string result = "";
    int sz, i;

    sz = sizeof(str);
    for (i = 0; i < sz; i++) {
        if (pcre_match(str[i..i], "^[a-zA-Z0-9" + allowed + "]$") == 0) {
            result += sprintf("%%%02X", str[i]);
        } else {
            result += str[i..i];
        }
    }

    return result;
}

string url_decode(string str) {
    mixed *arrs;
    string *parts;
    int *matches;
    int sz, i;

    // Use pcre_assoc to find percent-encoded sequences
    arrs = pcre_assoc(str, ({ "%[A-Fa-f0-9]{2}" }), ({ 1 }));

    parts = arrs[0];
    matches = arrs[1];

    sz = sizeof(parts);
    for (i = 0; i < sz; i++) {
        if (matches[i] == 1) {
            int char;
            sscanf(parts[i], "%%%x", char);
            parts[i] = sprintf("%c", char);
        }
    }

    return implode(parts, "");
}

mapping parse_url(string str) {
    string location, protocol, host, path, secure_part;
    int port, secure ;
    string *matches, port_string ;
    mapping result = ([]) ;

    // Use regex to parse the URL and determine the protoco
    matches = pcre_extract(str, "^(http|ws)(s?)://([^:/]+):?(\\d+)?(/.*)?$");
    if (!sizeof(matches))
        return 0 ;

    protocol = matches[0];
    secure = matches[1] == "s";
    host = matches[2];
    if(sizeof(matches) >= 4)
        port_string = matches[3] ;
    if(sizeof(matches) >= 5)
        path = matches[4] ;

    if(!strlen(port_string))
        port = secure ? 443 : 80 ;
    else
        port = to_int(port_string) ;

    result = ([
        "protocol" : protocol + (secure ? "s" : ""),
        "secure" : secure,
        "host" : host,
        "port" : port,
        "path" : path || "/",
    ]) ;

    return result ;
}

varargs int find_marker(mixed buf, mixed marker) {
    int i, sz, marker_sz, direction, start;
    string type = typeof(buf);
    mixed marker_test;

    sz = sizeof(buf);
    if(bufferp(buf))
        return bufsrch(buf, marker) ;

    if(!stringp(buf))
        return -1 ;

    if(!stringp(marker))
        return -1 ;

    return strsrch(buf, marker) ;
}

varargs int bufsrch(buffer buf, mixed str) {
    int buf_sz ;
    int sub_buf_sz ;
    mixed sub_buf ;
    int start, direction ;

    buf_sz = sizeof(buf);
    if(stringp(str))
        sub_buf = to_binary(str);
    else
        sub_buf = str ;

    sub_buf_sz = sizeof(sub_buf);

    // If searching from the end, set the start index to the end of the buffer
    start = 0 ;

    // If sub-buffer is larger than the buffer or start is out of bounds, return -1
    if (sub_buf_sz > buf_sz || start < 0)
        return -1;

    // Iterate over the buffer to find the sub-buffer
    for (int i = start; i < buf_sz - sub_buf_sz + 1; i++) {
        if (buf[i..i + sub_buf_sz - 1] == sub_buf) {
            return i;
        }
    }

    return -1;
}

string binary_to_hex(buffer buf) {
    string hex = "";
    int len = sizeof(buf);

    for (int i = 0; i < len; i++) {
        hex += sprintf("0x%02x ", buf[i]);
    }

    // Remove the trailing and space
    if (len > 0) {
        hex = trim(hex) ;
    }

    return hex;
}

void cache_response(string file, mixed response) {
    int fs = file_size(file) ;
    int sz = sizeof(response) ;
    int max = get_config(__MAX_BYTE_TRANSFER__) ;
    int start ;
    int result ;
    int x, y;

    _log(3, "Writing cache to file: "+file) ;
    _log(4, "Writing response: %O", binary_to_hex(response)) ;
    _log(3, "Filename " + file + " exists (size: %d): " + (file_exists(file) ? "true" : "false"), fs) ;

    if(bufferp(response)) {
        response = to_string(response) ;
    }
    if(!stringp(response)) {
        _log(4, "response is not a buffer or string") ;
        return ;
    }

    result = write_file(file, response) ;
    if(!result) {
        _log(4, "Failed to write to file: "+file) ;
        return ;
    }

    _log(3, "Wrote %d bytes to file: %s, new file size: %d", sz, file, file_size(file)) ;
}

mixed read_cache(string file) {
    mixed response = "" ;
    int chunk_size = get_config(__MAX_BYTE_TRANSFER__) ;
    int max = get_config(__MAX_STRING_LENGTH__) ;

    int curr = 0 ;
    int sz = file_size(file) ;
    string input ;
    int bytes_to_read ;

    // _log(1, "File size of " + file + ": " + sz) ;

    if (sz < 1) {
        return response ;
    }

    if(sz > max) {
        // _log(1, "File size exceeds maximum allowed size") ;
        return 0 ;
    }

    while (curr < sz) {
        bytes_to_read = (sz - curr) < chunk_size ? (sz - curr) : chunk_size;
        // _log(1, "Reading " + bytes_to_read + " bytes from position " + curr) ;

        while(curr < sz) {
            input = read_bytes(file, curr, bytes_to_read) ;

            if (!input) {
                // _log(1, "Failed to read bytes at position " + curr) ;
                break ; // Exit loop if read_bytes fails
            }

            response += input ;
            curr += chunk_size ;
        }
    }

    // write_file(file+".2", response) ;
    _log(3, "Bytes read: %d", sizeof(response)) ;
    return response ;
}

protected nomask string http_time_string(mapping client) {
    int now = time() ;
    mixed *lt = localtime(now), *gt ;
    int off = lt[LT_GMTOFF] ;

    now += off + (-1 * lt[LT_ISDST]) * 3600 ;
    return strftime("%a, %d %b %Y %H:%M:%S GMT", now) ;
}

protected nomask int http_content_length(mapping client) {
    mixed body ;
    buffer buf ;

    body = client["http"]["response"]["body"] ;

    if(!body)
        return 0 ;

    if(stringp(body))
        buf = to_binary(body) ;
    else if(bufferp(body))
        buf = body ;

    if(!bufferp(buf))
        return -1 ;

    return sizeof(body) ;
}

protected nomask void set_option(string key, mixed value) {
    _log(3, "Setting option %s to %O", key, value) ;

    options[key] = value ;
}

protected nomask mixed get_option(string key) {
    return options[key] ;
}

buffer to_binary(string str) { return string_encode(str, "UTF-8"); }
string to_string(buffer buf) { return string_decode(buf, "UTF-8"); }
