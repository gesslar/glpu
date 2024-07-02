// /adm/daemons/modules/http.c
// Modules for HTTP servers/clients
// Created:     2024/07/01: Gesslar
// Last Change: 2024/07/01: Gesslar
//
// 2024/07/01: Gesslar - Created

#include <daemons.h>
#include <http.h>

string url_encode(string str) ;
string url_decode(string str) ;

buffer to_binary(string str) ;
string to_string(buffer buf) ;

mapping parse_http(string str) ;
mapping parse_http_request(string str) ;
mapping parse_headers(string str) ;
mapping parse_body(string str, string content_type) ;
mapping parse_route(string str) ;
mapping parse_query(string str) ;

mapping parse_http(string str) {
    string request, header, body ;
    mapping result = ([]) ;
    int start_cursor, end_cursor ;
    string content_type ;

    end_cursor = strsrch(str, "\r\n\r\n");

    if(end_cursor == -1)
        return 0 ;

    header = str[0..end_cursor-1] ;
    start_cursor = strsrch(header, "\r\n") ;
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

mapping parse_headers(string str) {
    mapping headers = ([]);
    string* header_lines;
    string header_line;
    string header_name;
    string header_value;
    int i;

    header_lines = explode(str, "\r\n");

    for (i = 1; i < sizeof(header_lines); i++) {
        header_line = header_lines[i];

        if (header_line == "") {
            break;
        }

        if(sscanf(header_line, "%s: %s", header_name, header_value) == 2) {
            mixed t ;

            header_name = lower_case(header_name) ;

            if(sscanf(header_value, "%d", t)) {
                headers[header_name] = t ;
            } else if(sscanf(header_value, "%f", t)) {
                headers[header_name] = t ;
            } else {
                headers[header_name] = lower_case(header_value);
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
            // Treat as plain text
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

buffer to_binary(string str) { return string_encode(str, "UTF-8"); }
string to_string(buffer buf) { return string_decode(buf, "UTF-8"); }
