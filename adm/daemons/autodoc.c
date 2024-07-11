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

private nosave string *dirs_to_check = ({});
private nosave string *files_to_check = ({});

private nosave int dir_call_out = 0;
private nosave int file_call_out = 0;

private nomask int check_running();
private nomask void finish_scan();
private nomask void parse_file(string file);

private nosave nomask string jsdoc_function_regex,
       *jsdoc_function_ignore_tags,
       *tags, tag_regex,
       continue_regex,
       function_detect_regex ;

private nosave nomask mapping docs = ([]);

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

    if(check_running() == true)
        return;

    start_time = time_frac();
    dirs_to_check = mud_config("AUTODOC_DIRS");

    docs = ([]);
    files_to_check = ({});

    dir_call_out = call_out_walltime("check_dir", dir_delay);

    write(repeat_string("\n", 20)) ;
}

void check_dir() {
    string *files;
    string source_dir ;
    mapping docs = ([]);

    if(check_running() == false)
        return finish_scan();

    source_dir = dirs_to_check[0];
    source_dir = append(source_dir, "/");
    files = get_dir(append(source_dir, "*.c"));
    files = map(files, (: $2 + $1 :), source_dir);
    files_to_check += files;

    file_call_out = call_out_walltime("check_file", file_delay);

    dirs_to_check = dirs_to_check[1..];
    if(sizeof(dirs_to_check) == 0)
        dir_call_out = 0;
    else
        dir_call_out = call_out_walltime("check_dir", dir_delay);
}

void check_file() {
    string file;

    if(check_running() == false)
        return finish_scan();

    file = files_to_check[0];
    files_to_check = files_to_check[1..];

    if(sizeof(files_to_check) == 0)
        file_call_out = 0;
    else
        file_call_out = call_out_walltime("check_file", file_delay);

    parse_file(file);
}

void parse_file(string file) {
    string *lines;
    int num, max;
    int in_jsdoc = 0;
    string doc_type, function_tag, function_name, tag, info, line, out, output_file;
    int j;
    string *result;

    if(!file_exists(file))
        return;

    lines = explode(read_file(file), "\n");

    max = sizeof(lines);
    num = 0;
    out = "" ;

    for(; num < max; num++) {
        int jsdoc_function_found;
        string current_tag;
        mapping curr = null ;

        line = lines[num];

        // Check for the start of a JSDoc-style comment
        if(pcre_match(line, "^/\\*\\*") == true) {
            in_jsdoc = 1;
            jsdoc_function_found = 0;
            current_tag = null;
            curr = ([]); // Reset output buffer for new comment block
        }

        // Process lines within the JSDoc comment block
        if(in_jsdoc == 1) {
            /* ************************************************************* */
            /* THIS IS THE END OF THE JSDOC COMMENT BLOCK                    */
            /* ************************************************************* */
            if(pcre_match(line, "^\\s*\\*/") == true) {
                in_jsdoc = 0;

                /* ********************************************************* */
                /* NOW THAT WE HAVE FOUND THE END OF THE JSDOC COMMENT       */
                /* BLOCK, WE MUST LOOK FOR THE FUNCTION DEFINITION AND       */
                /* EXTRACT IT FOR OUR DOCUMENTATION.                         */
                /* ********************************************************* */
                for(j = num + 1; j < max; j++) {
                    line = lines[j];
                    result = pcre_extract(line, function_detect_regex);
                    if(sizeof(result) == 1) {
                        function_name = result[0];
                        if(function_name == function_tag) {
                            // Ensure the line ends with '{' and remove it
                            line = chop(trim(line), "{", -1);
                            out += "Function Definition: " + line + "\n";
                            num = j; // Move the outer loop index to the function definition line
                            break;
                        }
                    }
                }

                // If a matching function is not found, skip to the next doc/function check
                if(out == "") {
                    continue;
                }

                // Write the extracted information to the appropriate directory
                // TODO: Change the below to ultimately add to a mapping as docs[function_type][function_name]
                // output_file = "/doc/autodoc/" + "dest_dir" + "/" + function_name + ".txt";
                // assure_dir("/doc/autodoc/" + dest_dir);
                // write_file(output_file, out, 1);

                // Print the extracted information for debugging purposes
            } else {
                string *matches ;
                /* ********************************************************* */
                /* THIS IS THE BEGINNING OF THE JSDOC COMMENT BLOCK          */
                /* Format: @function_type function_name                      */
                /* ********************************************************* */
                // Check for the first tag to determine the document type and
                matches = pcre_extract(line, jsdoc_function_regex) ;
                if(sizeof(matches) > 0) {
                    /* DOC_TYPE WILL BE THE CURRENT CATEGORY UNDER docs([])  */
                    doc_type = matches[0];
                    /* UNLESS IT'S IN THE IGNORE LIST, THEN, NEVERMIND       */
                    if(of(doc_type, jsdoc_function_ignore_tags) > -1) {
                        in_jsdoc = 0;
                        continue;
                    }

                    function_tag = matches[1];
                    // we can continue now and parse the rest in the next
                    // iteration
                    continue;
                }

                /* ********************************************************* */
                /* THIS IS THE MIDDLE OF THE JSDOC COMMENT BLOCK             */
                /* WE ARE NOW LOOKING FOR A TAG                              */
                /* Format: @tag_name tag_info                                */
                /* WHERE tag_name IS ONE OF THE TAGS IN tags({})             */
                /* ********************************************************* */
                if(stringp(function_tag)) {
                    // If no current tag is found, try to match a tag
                    if(current_tag == null) {
                        if(pcre_match(line, tag_regex)) {
                            matches = pcre_extract(line, tag_regex) ;
                            if(sizeof(matches) > 0) {
                                out += sprintf("%s: %s\n",
                                    capitalize(matches[0]),
                                    matches[1]
                                );
                                current_tag = matches[0];
                            }
                        }
                    /* ***************************************************** */
                    /* WE HAVE A TAG! NOW WE NEED TO GET THE INFO PERTAINING */
                    /* TO THAT TAG.                                          */
                    /* ***************************************************** */
                    } else {
                        // It looks like we have encountered a new tag.
                        // If so, we need to reset so we can get the new tag's
                        // information.
                        if(pcre_match(line, "^\\s*\\*\\s+@") == true) {
                            current_tag = null;
                            num--;
                            continue;
                        }

                        // If we have a blank line, we can reset the current
                        // tag, we may have unexpectedly reached the end of the
                        // JSDoc comment block.
                        if(pcre_match(line, "^\\s*$") == true) {
                            current_tag = null;
                            continue;
                        }

                        // If we haven't reached a new tag and we haven't
                        // reached the end of the JSDoc comment block, we can
                        // assume we are still parsing additional information
                        // for the current tag. We can append the current line
                        // to the current tag's information.
                        if(pcre_match(line, continue_regex)) {
                            matches = pcre_extract(line, continue_regex) ;
                            if(sizeof(matches) > 0) {
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
