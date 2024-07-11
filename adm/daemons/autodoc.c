// /adm/daemons/autodoc.c
// This is Gesslar's attempt at an autodocumentation system.
//
// Created:     2024/02/18: Gesslar
// Last Change: 2024/02/18: Gesslar
//
// 2024/02/18: Gesslar - Created

inherit STD_DAEMON;
inherit M_LOG ;

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
    debug(repeat_string("\n", 20)) ;
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
    string doc_type, function_tag, function_def, tag, info, line ;
    int j;

    _log("Parsing file: %s", file) ;

    if(!file_exists(file))
        return;

    lines = explode(read_file(file), "\n");

    max = sizeof(lines);
    num = 0;

    for(; num < max; num++) {
        int jsdoc_function_found;
        string current_tag;
        mapping curr ;
        string *matches ;
        string *tag_data ;
        int new_tag_found ;

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

            new_tag_found = 0;
            current_tag = null;
        }

        line = lines[num];

        // Check for the start of a JSDoc-style comment
        if(pcre_match(line, "^/\\*\\*") == true) {
            in_jsdoc = 1;
            jsdoc_function_found = 0;
            current_tag = null;
            tag_data = null ;
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
                    matches = pcre_extract(line, function_detect_regex);
                    if(sizeof(matches) == 1) {
                        function_def = matches[0];
                        if(function_def == function_tag) {
                            // Ensure the line ends with '{' and remove it
                            line = trim(chop((line), "{", -1));
                            curr["function_def"] = line ;
                            // Move the outer loop index to the function
                            //  definition line
                            num = j;
                            break;
                        }
                    }
                }

                // If a matching function is not found, skip to the next doc/
                // function check
                if(!sizeof(curr)) {
                    continue;
                }

                /* ********************************************************* */
                /* NOW THAT WE HAVE THE FUNCTION DEFINITION, WE CAN ADD THE  */
                /* DOCUMENTATION TO THE APPROPRIATE LOCATION IN THE docs([]) */
                /* ********************************************************* */
                // Write the extracted information to the appropriate directory
                // TODO: Change the below to ultimately add to a mapping as docs[function_type][function_name]
                // output_file = "/doc/autodoc/" + "dest_dir" + "/" + function_name + ".txt";
                // assure_dir("/doc/autodoc/" + dest_dir);
                // write_file(output_file, out, 1);

                if(!of(doc_type, docs))
                    docs[doc_type] = ([]);

                docs[doc_type][function_tag] = curr ;
                printf("Docs for %s: %O\n", function_tag, docs[doc_type][function_tag]) ;
            } else {
                /* ********************************************************* */
                /* THIS IS THE BEGINNING OF THE JSDOC COMMENT BLOCK          */
                /* Format: @function_type function_tag                       */
                /* ********************************************************* */
                // Check for the first tag to determine the document type and
                if(!pcre_match(line, tag_regex) && pcre_match(line, jsdoc_function_regex)) {
                    matches = pcre_extract(line, jsdoc_function_regex) ;
                    if(sizeof(matches) > 0) {
                        /* DOC_TYPE WILL BE THE CURRENT CATEGORY UNDER docs([])  */
                        /* UNLESS IT'S IN THE IGNORE LIST, THEN, NEVERMIND       */
                        if(of(matches[0], jsdoc_function_ignore_tags)) {
                            in_jsdoc = 0;
                            continue;
                        }

                        doc_type = matches[0];
                        function_tag = matches[1];
                        _log("\e<0112>Found function name: \e<ul1>%s\e<res>", function_tag) ;
                        // we can continue now and parse the rest in the next
                        // iteration
                        continue;
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
                                current_tag = matches[0];
                                tag_data = ({ matches[1] });
                                _log("\e<0202>=>\e<res> Found tag: \e<0202>\e<ul1>%s\e<res>", current_tag) ;
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
                            _log(1, "\e<0032>=>\e<res> Found new tag, resetting tag to nothing") ;
                            new_tag_found = 1 ;
                            num--;
                            continue;
                        }

                        // If we have a blank line, we can reset the current
                        // tag, we may have unexpectedly reached the end of the
                        // JSDoc comment block.
                        if(pcre_match(line, "^\\s*$") == true) {
                            _log(1, "\e<0160>=>\e<res> Found blank line, resetting tag to nothing") ;
                            current_tag = null;
                            tag_data = null;
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
                                _log("\e<0220>=>\e<res> Found more info for tag: \e<0220>\e<ul1>%s\e<res>", current_tag) ;
                                tag_data += ({ matches[0] });
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
