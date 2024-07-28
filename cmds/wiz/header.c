inherit STD_CMD;

string *applies = ({});
string *excluded_functions = ({
    "mudlib_setup", "setup",
    "mudlib_unsetup", "unsetup",
    "main",
});
string excluded_pattern = "^(pre|post)_(setup|unsetup)_\\d+$";
string function_detect_regex = "^\\s*(public|protected|private|nomask|varargs)*\\s*"
    "(int|float|void|string|object|mixed|mapping|array|buffer|function)\\s*"
    "\\*?\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\(.*?\\)\\s*(\\{|;)?";

// Forward declarations
void setup();
mixed main(object tp, string str);
string generate_header(string path, string base_name);
string parse_file(string file);
int is_function_declaration(string line);
string extract_function_name(string line);
int should_include_function(string func_name);
string format_function(string line);
int is_potential_function_start(string line) ;

void setup() {
    string *apply_files;
    string file, content, apply;

    help_text =
        "This command scans a directory or single file for .c files and generates a header file "
        "containing function prototypes. It excludes certain system functions and "
        "applies. Usage: generate_header <directory|file>";
    usage_text = "generate_header <directory|file>";

    apply_files = get_dir("/doc/driver/apply/*");
    apply_files -= ({ ".", "..", ".gitignore" }) ;
    excluded_functions += apply_files;
}

mixed main(object tp, string str) {
    string cwd, resolved_path;
    string include_dir ;
    string *files, file ;
    int sz ;

    if (!str) {
        return _error(usage_text);
    }

    cwd = tp->query_env("cwd");
    resolved_path = resolve_path(cwd, str);

    sz = file_size(resolved_path);
    if(sz == -1) {
        return _error("The specified path does not exist: %s", resolved_path);
    }

    if(sz != -2 && sz == 0) {
        return _error("The specified path is empty: %s", resolved_path);
    }

    if(sz == -2) {
        files = get_dir(resolved_path + "/*.c");
        files = map(files, (: $(resolved_path) + $1 :));
        include_dir = resolved_path + "/include";
    } else {
        files = ({ resolved_path });
        include_dir = dir_file(resolved_path)[0] + "include";
    }

    if(!assure_dir(include_dir)) {
        return _error("Failed to create include directory: %s", include_dir);
    }

    printf("Include directory: %s\n", include_dir);

    foreach(file in files) {
        string *dir_parts = dir_file(file);
        string base_name = chop(dir_parts[1], ".c", -1) ;
        string output_file = include_dir + "/" + base_name + ".h";
        string header_content = generate_header(file, base_name) ;

        if(!header_content) {
            _error("Failed to generate header content for file: %s", file);
            continue ;
        }

        if(!write_file(output_file, header_content, 1)) {
            _error("Failed to write header file: %s", output_file);
            continue ;
        }

        _ok("Header file generated successfully: %s", output_file);
    }

    return 1;
}

string generate_header(string path, string base_name) {
    string header = "";
    string *files;
    string guard_name = upper_case(base_name) + "_H";

    header += sprintf("#ifndef __%s__\n", guard_name);
    header += sprintf("#define __%s__\n\n", guard_name);

    header += parse_file(path);

    header += sprintf("\n#endif // __%s__\n", guard_name);
    return header;
}

string parse_file(string file) {
    string content, file_header = "";
    string *lines;
    string current_function = "";
    string func_name;
    mapping processed_functions = ([]);
    int in_function = 0;
    int brace_count = 0;
    mixed open_braces, close_braces;
    int in_comment = 0;
    int in_if_0_block = 0;
    int potential_function_start = 0;
    string potential_function = "";

    content = read_file(file);
    lines = explode(content, "\n");

    _info("Parsing file: %s", file);

    foreach(string line in lines) {
        // Skip empty lines and full-line comments
        if (trim(line) == "" || pcre_match(trim(line), "^//")) continue;

        // Handle start/end of multi-line comments
        if (pcre_match(line, "/\\*")) in_comment = 1;
        if (pcre_match(line, "\\*/")) {
            in_comment = 0;
            continue;
        }
        if (in_comment) continue;

        // Handle #if 0 blocks
        if (pcre_match(trim(line), "^#if 0")) in_if_0_block = 1;
        if (pcre_match(trim(line), "^#endif") && in_if_0_block) {
            in_if_0_block = 0;
            continue;
        }
        if (in_if_0_block) continue;

        if (!in_function) {
            if (potential_function_start) {
                potential_function += " " + trim(line);
                if (pcre_match(potential_function, ".*\\)\\s*(\\{|;)")) {
                    current_function = potential_function;
                    func_name = extract_function_name(current_function);
                    if (func_name != "" && should_include_function(func_name) && !processed_functions[func_name]) {
                        _info("Including multi-line function: %s", func_name);
                        file_header += format_function(current_function) + "\n";
                        processed_functions[func_name] = 1;
                    }
                    potential_function_start = 0;
                    potential_function = "";
                    if (strsrch(line, "{") != -1) {
                        in_function = 1;
                        brace_count = 1;
                    }
                }
            } else if (is_potential_function_start(line)) {
                potential_function_start = 1;
                potential_function = line;
            } else if (is_function_declaration(line)) {
                // ... (existing code for single-line function declarations)
            }
        } else {
            current_function += " " + line;
            open_braces = pcre_match_all(line, "\\{");
            close_braces = pcre_match_all(line, "\\}");
            brace_count += sizeof(open_braces) - sizeof(close_braces);
            if (brace_count == 0) {
                in_function = 0;
                if (func_name != "" && should_include_function(func_name) && !processed_functions[func_name]) {
                    _info("Including multi-line function: %s", func_name);
                    file_header += format_function(current_function) + "\n";
                    processed_functions[func_name] = 1;
                }
                current_function = "";
            }
        }
    }
    return file_header;
}

int is_function_declaration(string line) {
    int result = pcre_match(line, function_detect_regex);
    _info("pcre_match result for is_function_declaration: %d", result);
    if (!result) {
        // Check for potential start of multi-line declaration
        result = is_potential_function_start(line);
    }
    _info("is_function_declaration: %s -> %d", line, result);
    return result;
}

string extract_function_name(string line) {
    string *matches;

    _info("Attempting to extract function name from: %s", line);
    matches = pcre_extract(line, function_detect_regex);
    _info("pcre_extract result: %O", matches);

    if(sizeof(matches) > 2) {  // Changed from > 3 to > 2
        string func_name = matches[2];  // Changed from matches[3] to matches[2]
        _info("Extracted function name: %s", func_name);
        return func_name;
    }
    _info("Failed to extract function name from: %s", line);
    return "";
}

int should_include_function(string func_name) {
    if (member_array(func_name, applies) != -1) {
        _info("Excluding %s (in applies)", func_name);
        return 0;
    }
    if (member_array(func_name, excluded_functions) != -1) {
        _info("Excluding %s (in excluded_functions)", func_name);
        return 0;
    }
    if (pcre_match(func_name, excluded_pattern)) {
        _info("Excluding %s (matches excluded_pattern)", func_name);
        return 0;
    }
    _info("Including %s", func_name);
    return 1;
}

string format_function(string line) {
    string formatted_line;
    int brace_index;
    string comments ;

    _info("Formatting function: %s", line);

    // Remove everything after and including the opening brace
    brace_index = strsrch(line, "{");
    if (brace_index != -1) {
        formatted_line = line[0..brace_index-1];
    } else {
        formatted_line = line;
    }

    _info("After removing braces: %s", formatted_line);

    // Remove any default parameter definitions
    formatted_line = pcre_replace(formatted_line, "(:\\s*\\([^)]*\\))", ({ "" }));
    _info("After removing default parameters: %s", formatted_line);

    // Remove newlines and extra whitespace
    formatted_line = pcre_replace(formatted_line, "(\\s+)", ({ " " }));
    _info("After removing extra whitespace: %s", formatted_line);

    // Preserve inline comments
    comments = "";
    while (pcre_match(formatted_line, "/\\*.*?\\*/")) {
        string *parts = pcre_extract(formatted_line, "(.*?)(/\\*.*?\\*/)(.*)");
        if (sizeof(parts) > 3) {
            formatted_line = parts[1] + parts[3];
            comments += " " + parts[2];
        }
    }

    // Ensure there's a semicolon at the end
    formatted_line = trim(formatted_line);
    if (formatted_line[<1] != ';') {
        formatted_line += ";";
    }

    // Add back preserved comments
    formatted_line += comments;

    // Only add 'varargs' if there's a default parameter or variadic argument
    if ((strsrch(line, ":(") != -1 || strsrch(line, "...") != -1) &&
        strsrch(formatted_line, "varargs") == -1) {
        string visibility = "";
        if (pcre_match(formatted_line, "^(public|protected|private)")) {
            sscanf(formatted_line, "%s %s", visibility, formatted_line);
            formatted_line = visibility + " varargs " + formatted_line;
        } else {
            formatted_line = "varargs " + formatted_line;
        }
    }

    _info("Final formatted function: %s", formatted_line);
    return formatted_line;
}

int is_potential_function_start(string line) {
    return pcre_match(line, "^\\s*(public|protected|private|nomask|varargs)*\\s*"
        "(int|float|void|string|object|mixed|mapping|array|buffer|function)\\s*"
        "\\*?\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\(?");
}
