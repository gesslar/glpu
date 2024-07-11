// /adm/daemons/autodoc.c
// This is Gesslar's attempt at an autodocumentation system.
//
// Created:     2024/02/18: Gesslar
// Last Change: 2024/02/18: Gesslar
//
// 2024/02/18: Gesslar - Created

inherit STD_DAEMON;

private nosave float dir_delay = 0.02;
private nosave float file_delay = 0.01;

private nosave float start_time = 0;

private nosave mixed *dirs_to_check = ({});
private nosave mixed *files_to_check = ({});

private nosave int dir_call_out = 0;
private nosave int file_call_out = 0;

private nomask int check_running();
private nomask void finish_scan();
private nomask void parse_file(string *file);

string jsdoc_function_regex,
       *jsdoc_function_ignore_tags,
       *tags, tag_regex,
       continue_regex,
       function_detect_regex ;


void setup() {
    jsdoc_function_regex = "^\\s\\*\\s+@(\\w+)\\s+(\\w+)\\s*$" ;

    jsdoc_function_ignore_tags = ({ "function" }) ;

    tags = ({ "description", "param", "returns", "example" }) ;
    tag_regex = sprintf("^\\s*\\*\\s+@(%s)\\s+(.*)$",
        implode(tags, "|")) ;

    continue_regex = "^\\s\\*\\s+(.*)$" ;

    function_detect_regex =
    "^(?:public|protected|private)?\\s*"
    "(?:nomask|varargs)?\\s*(?:nomask|varargs)?\\s*"
    "(?:int|float|void|string|object|mixed|mapping|array|buffer|function)\\s*"
    "\\*?\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\("
    ;
}

void autodoc_scan() {
    mixed *config;

    if (check_running() == true)
        return;

    start_time = time_frac();
    dirs_to_check = mud_config("AUTODOC_DIRS");

    dir_call_out = call_out_walltime("check_dir", dir_delay);

    write(repeat_string("\n", 20)) ;
}

void check_dir() {
    mixed *current;
    string *files;
    string source_dir, dest_dir;
    mapping docs = ([]);

    if (check_running() == false)
        return finish_scan();

    current = dirs_to_check[0];
    source_dir = current[0];
    dest_dir = current[1];

    assure_dir(dest_dir);

    source_dir = append(source_dir, "/");
    files = get_dir(append(source_dir, "*.c"));
    files = map(files, (: $2 + $1 :), source_dir);
    foreach (string file in files)
        files_to_check += ({ ({ file, dest_dir }) });

    file_call_out = call_out_walltime("check_file", file_delay);
    dirs_to_check = dirs_to_check[1..];
    if (sizeof(dirs_to_check) == 0)
        dir_call_out = 0;
    else
        dir_call_out = call_out_walltime("check_dir", dir_delay);
}

void check_file() {
    string *file;

    if (check_running() == false)
        return finish_scan();

    file = files_to_check[0];
    files_to_check = files_to_check[1..];

    if (sizeof(files_to_check) == 0)
        file_call_out = 0;
    else
        file_call_out = call_out_walltime("check_file", file_delay);

    parse_file(file);
}

void parse_file(string *file_info) {
    string *lines;
    string file, dest_dir;
    int num, max;
    int in_jsdoc = 0;
    string doc_type, function_name, tag, info, line, out, output_file;
    int j;
    string *result;

    file = file_info[0];
    dest_dir = file_info[1];

    if (!file_exists(file))
        return;

    lines = explode(read_file(file), "\n");

    max = sizeof(lines);
    num = 0;
    out = "" ;

    for (; num < max; num++) {
        int jsdoc_function_found;
        string current_tag;

        line = lines[num];

        // Check for the start of a JSDoc-style comment
        if (pcre_match(line, "^/\\*\\*") == true) {
            in_jsdoc = 1;
            jsdoc_function_found = 0;
            current_tag = null;
            out = ""; // Reset output buffer for new comment block
        }

        // Process lines within the JSDoc comment block
        if (in_jsdoc == 1) {
            // Check for the end of a JSDoc comment block
            if (pcre_match(line, "^\\s*\\*/") == true) {
                in_jsdoc = 0;

                // Look ahead for the function definition
                for (j = num + 1; j < max; j++) {
                    line = lines[j];
                    result = pcre_extract(line, function_detect_regex);
                    if (sizeof(result) == 1) {
                        function_name = result[0];
                        if (function_name == tag) {
                            // Ensure the line ends with '{' and remove it
                            line = chop(trim(line), "{", -1);
                            out += "Function Definition: " + line + "\n";
                            num = j; // Move the outer loop index to the function definition line
                            break;
                        }
                    }
                }

                // If a matching function is not found, skip to the next doc/function check
                if (out == "") {
                    continue;
                }

                // Write the extracted information to the appropriate directory
                output_file = "/doc/autodoc/" + dest_dir + "/" + function_name + ".txt";
                assure_dir("/doc/autodoc/" + dest_dir);
                write_file(output_file, out, 1);

                // Print the extracted information for debugging purposes
            } else {
                string *matches, reg;
                // Check for the first tag to determine the document type and directory
                reg = jsdoc_function_regex;
                if (sizeof(matches = pcre_extract(line, reg)) > 0) {
                    doc_type = matches[0];
                    // If this is not the sort of function we are interested
                    // in, skip ahead to the next JSDoc block
                    if(member_array(doc_type, jsdoc_function_ignore_tags) > -1) {
                        in_jsdoc = 0;
                        continue;
                    }

                    dest_dir = doc_type;
                    tag = matches[1];
                    out += "Function: " + tag + "\n";
                    jsdoc_function_found = 1;
                    current_tag = null;
                    continue;
                }

                // Process other tags if the function has been found
                if (jsdoc_function_found == 1) {
                    // If no current tag is found, try to match a tag
                    if (current_tag == null) {
                        if (pcre_match(line, tag_regex)) {
                            if (sizeof(matches = pcre_extract(line, tag_regex)) > 0) {
                                out += sprintf("%s: %s\n", capitalize(matches[0]), matches[1]);
                                current_tag = matches[0];
                            }
                        }
                    } else {
                        // Process lines that belong to the current tag until
                        // a new tag or blank line is found
                        if (pcre_match(line, "^\\s*\\*\\s+@") == true) {
                            current_tag = null;
                            num--;
                            continue;
                        }

                        if (pcre_match(line, "^\\s*$") == true) {
                            current_tag = null;
                            continue;
                        }

                        // Append lines that are part of the current tag
                        if (pcre_match(line, continue_regex)) {
                            if (sizeof(matches = pcre_extract(line, continue_regex)) > 0) {
                                out += matches[0] + "\n";
                            }
                        }
                    }
                }
            }
        }
    }
}

void finish_scan() {
    debug("Autodoc scan finished in " + (time_frac() - start_time) + "ms.");
}

int check_running() {
    return sizeof(dirs_to_check) > 0 || sizeof(files_to_check) > 0;
}
