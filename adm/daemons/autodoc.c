/**
 * @file /adm/daemons/autodoc.c
28 * @description Autodocumentation system that parses JSDoc-style comments in
 *              LPC source files and generates documentation in a structured
 *              format.
 *
 * @created 2024/02/18 - Gesslar
 * @last_modified 2024/07/11 - Gesslar
 *
 * @history
 * 2024/02/18 - Gesslar - Created
 * 2024/07/11 - Gesslar - Entirely rewritten from Lima-style parsing to JSDoc
 */

inherit STD_DAEMON ;
inherit M_LOG ;

// Function prototypes
private nomask int check_running();
private nomask void finish_scan();
private nomask void parse_file(string file);
private nomask void write_markdown() ;

// Variables
private nosave nomask float dir_delay = 0.02 ;
private nosave nomask float file_delay = 0.01 ;

private nosave nomask float start_time = 0.0 ;

private nosave nomask string doc_root ;
private nosave nomask string *dirs_to_check = ({});
private nosave nomask string *files_to_check = ({});

private nosave nomask int scanning = null ;
private nosave nomask int writing = null ;

private nosave nomask string jsdoc_function_regex,
                            *jsdoc_function_ignore_tags,
                            *tags, tag_regex,
                             continue_regex,
                             function_detect_regex ;

private nosave nomask mapping docs = ([]);

void setup() {
    set_log_level(4) ;

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

public nomask void autodoc_scan() {
    mixed *config ;

    if(check_running() == true)
        return ;

    _log(1, "Starting autodoc scan") ;

    writing = false ;
    doc_root = mud_config("AUTODOC_ROOT") ;
    start_time = time_frac();
    dirs_to_check = mud_config("AUTODOC_SOURCE_DIRS");
    scanning = sizeof(dirs_to_check) ;

    docs = ([]);
    files_to_check = ({});

    debug(repeat_string("\n", 20)) ;

    call_out_walltime("check_dir", dir_delay);
}

private nomask void check_dir() {
    string *files ;
    string source_dir ;
    mapping docs = ([]);

    scanning -= 1 ;

    source_dir = dirs_to_check[0] ;
    source_dir = append(source_dir, "/");
    files = get_dir(append(source_dir, "*.c"));
    files = map(files, (: $2 + $1 :), source_dir);
    files_to_check += files ;
    scanning += sizeof(files) ;

    call_out_walltime("check_file", file_delay);

    dirs_to_check = dirs_to_check[1..] ;

    if(!sizeof(dirs_to_check))
        return finish_scan() ;

    call_out_walltime("check_dir", dir_delay);
}

private nomask void check_file() {
    string file ;
    int result ;

    scanning -= 1 ;
    file = files_to_check[0] ;
    files_to_check = files_to_check[1..] ;

    parse_file(file);

    if(!sizeof(files_to_check))
        return finish_scan() ;

    call_out_walltime("check_file", file_delay);
}

private nomask void parse_file(string file) {
    string *lines ;
    int num, max ;
    int in_jsdoc = 0 ;
    string doc_type, function_tag, function_def, tag, info, line ;
    int j ;

    if(!file_exists(file))
        return ;

    lines = explode(read_file(file), "\n");

    max = sizeof(lines);
    num = 0 ;

    for(; num < max ; num++) {
        int jsdoc_function_found ;
        string current_tag ;
        mapping curr ;
        string *matches ;
        string *tag_data ;
        int new_tag_found ;

        line = lines[num] ;

        // track the current tag and its data
        if(function_tag == "reverse_strsrch") {
            _log(2, "Current tag: %O", current_tag) ;
            _log(2, "Tag data: %O", tag_data) ;
        }

        // Check for the start of a JSDoc-style comment
        if(!in_jsdoc) {
            // Clear stuff since we've definitely exited the JSDoc comment block
            if(stringp(function_tag)) {
                function_tag = null ;
                jsdoc_function_found = 0 ;
                current_tag = null ;
                tag_data = null ;
            }

            if(pcre_match(line, "^/\\*\\*") == true) {
                in_jsdoc = 1 ;
                jsdoc_function_found = 0 ;
                current_tag = null ;
                tag_data = null ;
                curr = ([]); // Reset output buffer for new comment block
            } else {
                continue ;
            }
        } else {
            if(function_tag == "reverse_strsrch")
                _log(2, "We're still parsing the JSDoc comment block for function %s", function_tag) ;
        }

        // Process lines within the JSDoc comment block
        if(in_jsdoc == 1) {
            // If we encountered a new tag while iterating, we need to now save
            // the currently gathered information into curr([]) ;
            // And then keep on trucking!
            if(new_tag_found == 1) {
                curr = curr || ([]);
                if(nullp(tag_data))
                    tag_data = ({}); // Ensure we have an array to append to
                if(!of(current_tag, curr))
                    curr[current_tag] = ({ tag_data }) ;
                else
                    // Append the new tag data to the existing tag data (if any
                    curr[current_tag] += ({ tag_data }) ;

                new_tag_found = 0 ;
                current_tag = null ;
            }

            /* ************************************************************* */
            /* THIS IS THE END OF THE JSDOC COMMENT BLOCK                    */
            /* ************************************************************* */
            if(pcre_match(line, "^\\s*\\*/") == true) {
                in_jsdoc = 0 ;

                if(function_tag == "reverse_strsrch") {
                    _log(2, "We found the end of the JSDoc comment block for %s", function_tag) ;
                }

                // Record the last tag data
                if(current_tag != null) {
                    curr = curr || ([]);
                    if(nullp(tag_data))
                        tag_data = ({}); // Ensure we have an array to append to
                    if(!of(current_tag, curr))
                        curr[current_tag] = ({ tag_data }) ;
                    else
                        // Append the new tag data to the existing tag data (if any
                        curr[current_tag] += ({ tag_data }) ;
                }

                /* ********************************************************* */
                /* NOW THAT WE HAVE FOUND THE END OF THE JSDOC COMMENT       */
                /* BLOCK, WE MUST LOOK FOR THE FUNCTION DEFINITION AND       */
                /* EXTRACT IT FOR OUR DOCUMENTATION.                         */
                /* ********************************************************* */
                for(j = num + 1; j < max; j++) {
                    line = lines[j] ;
                    matches = pcre_extract(line, function_detect_regex);
                    if(sizeof(matches) == 1) {
                        function_def = matches[0] ;
                        if(function_def == function_tag) {
                            // Ensure the line ends with '{' and remove it
                            line = trim(chop((line), "{", -1));
                            curr["function_def"] = line ;
                            // Move the outer loop index to the function
                            //  definition line
                            num = j ;
                            break ;
                        }
                    }
                }

                // If a matching function is not found, skip to the next doc/
                // function check
                if(!of("function_def", curr)) {
                    in_jsdoc = 0 ;
                    continue ;
                }

                /* ********************************************************* */
                /* NOW THAT WE HAVE THE FUNCTION DEFINITION, WE CAN ADD THE  */
                /* DOCUMENTATION TO THE APPROPRIATE LOCATION IN THE docs([]) */
                /* ********************************************************* */
                if(!of(doc_type, docs))
                    docs[doc_type] = ([]);

                docs[doc_type][function_tag] = curr ;
                in_jsdoc = 0 ;
                if(function_tag == "reverse_strsrch") {
                    _log(2, "We made it to the end of the JSDoc comment block for %s", function_tag) ;
                }
                continue ;
            } else {
                /* ********************************************************* */
                /* THIS IS THE BEGINNING OF THE JSDOC COMMENT BLOCK          */
                /* Format: @function_type function_tag                       */
                /* ********************************************************* */
                // Check for the first tag to determine the document type and
                if(!pcre_match(line, tag_regex) &&
                    pcre_match(line, jsdoc_function_regex)) {
                    matches = pcre_extract(line, jsdoc_function_regex) ;
                    if(sizeof(matches) > 0) {
                        /* ************************************************* */
                        /* DOC_TYPE WILL BE THE CURRENT CATEGORY UNDER       */
                        /* docs([]) UNLESS IT'S IN THE IGNORE LIST, THEN,    */
                        /* NEVERMIND.                                        */
                        /* ************************************************* */
                        if(of(matches[0], jsdoc_function_ignore_tags)) {
                            in_jsdoc = 0 ;
                            continue ;
                        }

                        doc_type = matches[0] ;
                        function_tag = matches[1] ;
                        // we can continue now and parse the rest in the next
                        // iteration
                        continue ;
                    }
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
                                current_tag = matches[0] ;
                                tag_data = ({ matches[1] });
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
                            // Reset the current tag and tag data
                            new_tag_found = 1 ;
                            num--;
                            continue ;
                        }

                        // If we have a blank line, we can reset the current
                        // tag, we may have unexpectedly reached the end of the
                        // JSDoc comment block.
                        if(pcre_match(line, "^\\s*$") == true) {
                            current_tag = null ;
                            tag_data = null ;
                            continue ;
                        }

                        // If we haven't reached a new tag and we haven't
                        // reached the end of the JSDoc comment block, we can
                        // assume we are still parsing additional information
                        // for the current tag. We can append the current line
                        // to the current tag's information.
                        if(pcre_match(line, continue_regex)) {
                            matches = pcre_extract(line, continue_regex) ;
                            if(sizeof(matches) > 0) {
                                tag_data += ({ matches[0] });
                            }
                        }
                    }
                }
            }
        } else {
            continue ;
        }
    }
}

private nomask void write_markdown() {
    mapping function_type ;
    mapping funcs ;
    mapping function_info ;
    function find_tag ;

    writing = true ;

    _log(2, "Function types: %O", keys(docs)) ;

    find_tag = function(string tag, string *tags) {
        int sz ;

        sz = sizeof(tags) ;
        while(sz--) {
            if(tags[sz] == tag)
                return tags[sz] ;
        }

        return null ;
    } ;

    foreach(function_type, funcs in docs) {
        string tag, line, element ;
        string func_name ;
        mapping func ;
        string output_dir ;

        // _log(2, "Function type: %O", function_type) ;
        // _log(2, "Function names: %O", keys(function_data)) ;

        output_dir = sprintf("%s/%s", doc_root, function_type) ;

        foreach(func_name, func in funcs) {
            string output_file ;
            string out = "" ;

            _log(2, "Function: %s", func_name) ;

            // We need a description and a function definition to continue,
            // otherwise we can skip this function.
            if(!of("description", func) || !of("function_def", func))
                continue ;

            output_file = sprintf("%s/%s.md", output_dir, func_name) ;
            _log(2, "Output file: %s", output_file) ;

            assure_dir(output_dir) ;

            /*
            // NAME section
            _log("Description: %O", func["description"]) ;
            // First the name of the function
            line = sprintf("    %s() - %s\n", func_name, func["description"][0]) ;
            if(strlen(line) > 76) {
                int dash_pos, space_pos ;
                string rest ;

                dash_pos = strsrch(line, "-") + 2 ;
                space_pos = reverse_strsrch(line[0..76], " ") ;
                line = line[0..space_pos] + "\n" +
                repeat_string(" ", dash_pos) + line[space_pos + 1..] ;
            }
            line = chop(line, "\n", -1) ;
            out += line + "\n\n" ;

            // If we have more than one line in the description, we will use
            // the remainder as the description.
            func["description"] = func["description"][1..] ;
            */
            // Now we want the synopsis, which is the function definition
            out += "### SYNOPSIS\n\n" ;
            out += sprintf("    %s\n", func["function_def"]) ;

            // Next we need to parse the parameters

            if(of("param", func)) {
                string *param ;
                out += "\n### PARAMETERS\n\n" ;
                foreach(param in func["param"]) {
                    string *matches, *arr_matches ;

                    line = implode(param, "\n") ;
                    while(strsrch(line, "  ") > -1) {
                        line = replace_string(line, "  ", " ") ;
                    }

                    matches = pcre_extract(line, "^\\{(.+?)\\}\\s+(\\w+)\\s+-\\s+(.*)$") ;
                    arr_matches = pcre_extract(matches[0], "^(.+?)\\[\\]$") ;
                    if(sizeof(arr_matches) > 0) {
                        matches[0] = sprintf("%s*", arr_matches[0]) ;
                    }

                    out += sprintf("    %s %s - %s\n", matches[0], matches[1], matches[2]) ;
                }
            }

            printf("Output\n%s", out) ;
            continue ;
            // Next we need to parse the parameters
            if(of("param", function_info)) {
                string *lines ;
                string *matches, *arr_matches ;
                out += "### PARAMETERS\n\n" ;

                // Extract type, name, and description
                lines = function_info["param"][element] ;
                line = implode(lines, "\n") ;
                while(strsrch(line, "  ") > -1) {
                    line = replace_string(line, "  ", " ") ;
                }
                line = trim(line) ;
                printf("Line: %O\n", line) ;
                matches = pcre_extract(line, "^\\{(.+?)\\}\\s+(\\w+)\\s+-\\s+(.*)$") ;
                printf("Matches: %O\n", matches) ;

                // For variables that might be arrays, let's alter the output
                // to reflect that.
                arr_matches = pcre_extract(matches[0], "^(.+?)\\[\\]$") ;
                if(sizeof(arr_matches) > 0) {
                    matches[0] = sprintf("%s*", arr_matches[0]) ;
                }

                out += sprintf("    %s %s - %s\n", matches[0], matches[1], matches[2]) ;
            }

            // Write the file
            write_file(output_file, out) ;
        }
    }

    writing = false ;
}

private nomask void finish_scan() {
    if(check_running() == true)
        return ;

    _log("Autodoc scan finished in %.2fs", time_frac() - start_time) ;

    printf("%O\n", docs["simul_efun"]["reverse_strsrch"]) ;

    // Save the documentation to a file
    // write_markdown() ;
}

public nomask int check_running() {
    return scanning > 0 || writing == true ;
}
