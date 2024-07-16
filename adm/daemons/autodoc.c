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
public nomask mixed autodoc_scan() ;
private nomask void finish_scan();
private nomask void parse_file(string file);
private nomask string generate_markdown(mapping doc) ;

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
private nosave int ci = false ;

void setup() {
    set_log_level(1) ;

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

public nomask void ci_build() {
    set_log_level(4) ;
    _log(1, "Autodoc is running in CI mode") ;
    ci = true ;
    autodoc_scan() ;
}

public nomask mixed autodoc_scan() {
    mixed *config ;

    if(check_running() == true)
        return "Autodoc is already running." ;

    _debug(repeat_string("\n", 20)) ;

    _log(1, "Starting autodoc scan") ;
    if(this_player() && devp(this_player()))
        _ok("Starting autodoc scan") ;

    writing = false ;
    doc_root = mud_config("AUTODOC_ROOT") ;
    start_time = time_frac();
    dirs_to_check = mud_config("AUTODOC_SOURCE_DIRS");
    scanning = sizeof(dirs_to_check) ;

    docs = ([]);
    files_to_check = ({});

    call_out_walltime("check_dir", dir_delay);

    return 1 ;
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

                curr["source_file"] = file ;
                docs[doc_type][function_tag] = curr ;
                in_jsdoc = 0 ;
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

private nomask string dash_wrap(string str, int width) ;

private nomask string generate_markdown(mapping func) {
    string out = "" ;
    string line ;
    mixed err ;

    err = catch {
        // We need a description and a function definition to continue,
        // otherwise we can skip this function.
        if(!of("description", func) || !of("function_def", func))
            return ;

        // Now we want the synopsis, which is the function definition
        out += "## SYNOPSIS\n\n" ;
        out += sprintf("    %s\n", func["function_def"]) ;

        // Next we need to parse the parameters

        if(of("param", func)) {
            mixed *params ;
            string *param ;

            out += "\n### PARAMETERS\n\n" ;

            params = func["param"] ;
            foreach(param in params) {
                string *matches, *arr_matches ;

                line = implode(map(param, (: trim :)), " ") ;
                matches = pcre_extract(line, "^(?:\\{)(.+)(?:\\}) (.+)$") ;
                line = dash_wrap(line, 76) ;
                out += sprintf("    %s %s\n", matches[0], matches[1]) ;
            }
        }

        if(of("returns", func)) {
            string *rets ;
            string *matches ;

            out += "\n### RETURNS\n\n" ;

            line = implode(func["returns"][0], " ") ;
            matches = pcre_extract(line, "^(?:\\{)(.+)(?:\\}) (.+)$") ;
            line = dash_wrap(line, 76) ;
            out += sprintf("    %s %s\n", matches[0], matches[1]) ;
        }

        if(of("description", func)) {
            string *desc ;

            out += "\n## DESCRIPTION\n\n" ;

            desc = func["description"][0] ;
            out += implode(desc, "\n") ;

            out += "\n" ;
        }

        // Write the file
        return out ;
    } ;
    if(err)
        log_file("system/autodoc", "Error generating markdown: " + err + "\n") ;

    return null ;
}

private nomask void generate_wiki() {
    string *function_types, function_type ;
    string home_content = "", doc_content = "" ;
    function generate_table ;

    writing = true ;

    // Generates a table in wiki format, with a maximum of 5 columns
    // and the content is a list of function names from left to right.
    generate_table = function(string function_type, string file_name, string *table) {
        string table_content = "";
        int max_columns = 3, column = 0;
        int column_width = 80 / max_columns;
        int sz = sizeof(table);

        // Create the first line of the table that is the border
        table_content += "|"+repeat_string(" |", max_columns)+"\n";
        table_content += "|"+repeat_string("---|", max_columns)+"\n";

        // Create the table rows
        foreach (string function_name in table) {
            if (column == 0)
                table_content += "|";

            table_content += sprintf("[%s](%s.html#%s)|",
                function_name,
                chop(file_name, ".c", -1),
                function_name
            ) ;

            column++;

            if (column == max_columns) {
                table_content += "\n";
                column = 0;
            }
        }

        // Add padding if the last row is not complete
        if (column != 0) {
            table_content += repeat_string(" |", max_columns - column) + "\n";
        }

        return table_content;
    };

    // Generate the wiki
    function_types = sort_array(keys(docs), 1) ;

    foreach(function_type in function_types) {
        string *function_names, function_name ;
        string current_source_file ;
        string *wiki_table = ({}) ;
        string dest_dir ;

        dest_dir = append(doc_root, function_type + "/") ;
        home_content += sprintf("## %s\n\n", function_type) ;

        function_names = sort_array(keys(docs[function_type]),
            function(string a, string b, string function_type) {
                string source_a, source_b ;

                source_a = docs[function_type][a]["source_file"] ;
                source_b = docs[function_type][b]["source_file"] ;

                if(source_a == source_b)
                    return strcmp(a, b) ;

                return strcmp(source_a, source_b) ;
            }, function_type
        ) ;

        foreach(function_name in function_names) {
            mixed md ;

            if(!current_source_file) {
                current_source_file = docs[function_type][function_name]["source_file"] ;
                doc_content = "" ;
            }

            // We're starting a new file name
            if(docs[function_type][function_name]["source_file"] != current_source_file) {
                string dest_file ;
                string *parts ;

                parts = dir_file(current_source_file) ;
                dest_file = append(dest_dir, chop(parts[1], ".c", -1) + ".md") ;
                _debug("Dest file = %s", dest_file) ;
                if(!assure_file(dest_file)) {
                    _error("Failed to create directory for: " + dest_file) ;
                    return ;
                }

                home_content += "### " + parts[1] + "\n\n" ;
                home_content += (*generate_table)(function_type, parts[1], wiki_table) ;
                home_content += "\n" ;

                write_file(dest_file, doc_content, 1) ;

                current_source_file = docs[function_type][function_name]["source_file"] ;

                wiki_table = ({ }) ;
                dest_file = append(chop(parts[1], ".c", -1), ".md") ;
            }

            if((md = generate_markdown(docs[function_type][function_name])) != null) {
                // Start with the function name
                doc_content += "# " + function_name + "\n\n" + md ;
                doc_content += "\n\n" ;
            }

            wiki_table += ({ function_name }) ;
        }

        write_file(append(doc_root, function_type+".md"), home_content, 1) ;
    }

    writing = false ;
}

private nomask string dash_wrap(string str, int width) {
    int dash_pos, space_pos ;
    string rest ;

    if(strlen(str) > width) {
        dash_pos = strsrch(str, "-") + 2 ;
        space_pos = reverse_strsrch(str[0..width], " ") ;
        str = str[0..space_pos] + "\n" +
        repeat_string(" ", dash_pos) + str[space_pos + 1..] ;
    }

    return str ;
}

private nomask void finish_scan() {
    float end_time ;

    if(check_running() == true)
        return ;

    end_time = time_frac() ;
    _log(1, "Autodoc scan finished in %.2fs", end_time - start_time) ;
    if(this_player() && devp(this_player()))
        _ok(sprintf("Autodoc scan finished in %.2fs", end_time - start_time)) ;

    // Save the documentation to a file
    // write_markdown() ;
    generate_wiki() ;

    end_time = time_frac() ;

    _log(1, "Autodoc has finished scanning and writing in %.2fs", end_time - start_time) ;
    if(this_player() && devp(this_player()))
        _ok("Autodoc has finished scanning and writing in %.2fs", end_time - start_time) ;

    if(ci == true) {
        shutdown(0) ;
    }
}

public nomask int check_running() {
    return scanning > 0 || writing == true ;
}
