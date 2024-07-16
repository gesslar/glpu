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
private nomask string generate_function_markdown(mapping doc) ;

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
    string doc_type, function_tag, function_def, line ;
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

private nomask string generate_function_markdown(mapping func) {
    string out = "" ;
    string line ;
    mixed err ;

    err = catch {
        // We need a description and a function definition to continue,
        // otherwise we can skip this function.
        if(!of("description", func) || !of("function_def", func))
            return null ;

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
                string *matches ;

                line = implode(map(param, (: trim :)), " ") ;
                matches = pcre_extract(line, "^(?:\\{)(.+)(?:\\}) (.+)$") ;
                line = dash_wrap(line, 76) ;
                out += sprintf("    %s %s\n", matches[0], matches[1]) ;
            }
        }

        if(of("returns", func)) {
            string *matches ;

            out += "\n### RETURNS\n\n" ;

            line = implode(func["returns"][0], " ") ;

            matches = pcre_extract(line, "^(?:\\{)(.+)(?:\\}) (.+)$") ;
            if(sizeof(matches) > 0) {
                line = dash_wrap(line, 76) ;
                out += sprintf("    %s %s\n", matches[0], matches[1]) ;
            }
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

private nomask string generate_function_type_markdown(string type, string source_file, mapping funcs) {
    string out = "" ;
    string line ;
    mixed err ;

    err = catch {
        string *function_names, function_name ;
        string dest_dir ;
        mapping func ;
        mapping interested  ;

        interested = filter(funcs, (: $2["source_file"] == $3 :), source_file) ;
        function_names = sort_array(keys(interested), 1) ;

        foreach(function_name in function_names) {
            string *parts ;
            string file ;

            func = interested[function_name] ;

            parts = dir_file(func["source_file"]) ;
            file = parts[1] ;

            line = sprintf("* [%s](%s/%s#%s)\n",
                function_name,
                type,
                chop(file, ".c", -1),
                function_name
            ) ;

            out += sprintf(line) ;
        }

        _debug("Generated function type markdown: %s", out) ;

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

    function_types = sort_array(keys(docs), 1) ;

    foreach(function_type in function_types) {
        string *function_names ;
        string func_type_file ;
        string current_source_file ;
        string dest_dir ;
        mapping funcs ;
        string *source_files, source_file ;

        _log(1, "Working on function type: %s", function_type) ;

        home_content += sprintf("## %s\n\n", function_type) ;

        funcs = docs[function_type] ;

        function_names = sort_array(keys(funcs), 1) ;

        _log(2, " Found %d functions of type %s", sizeof(function_names), function_type) ;

        dest_dir = append(doc_root, function_type + "/") ;
        _log(1, " Generating wiki for %s in %s", function_type, dest_dir) ;

        source_files = map(values(funcs), (: $1["source_file"] :)) ;
        source_files = distinct_array(source_files) ;
        source_files = sort_array(source_files, 1) ;

        func_type_file = append(doc_root, function_type + ".md") ;
        if(file_exists(func_type_file))
            rm(func_type_file) ;
        else
            assure_file(func_type_file) ;

        foreach(source_file in source_files) {
            string *parts, source_file_name ;
            mapping current_funcs ;
            string *curr_function_names, curr_function_name ;
            string dest_file ;
            string home_md ;

            parts = dir_file(source_file) ;
            source_file_name = chop(parts[1], ".c", -1) ;

            home_md = generate_function_type_markdown(function_type, source_file, funcs) ;
            home_content += sprintf("### %s\n\n", source_file_name) ;
            home_content += home_md + "\n" ;

            write_file(func_type_file, home_content, 1) ;

            dest_file = sprintf("%s%s/%s.md", doc_root, function_type, source_file_name) ;
            _log(2, "Dest file = %s", dest_file) ;
_debug("Funcs %O\n", funcs) ;
            current_funcs = filter(funcs, (: $2["source_file"] == $3 :), source_file) ;
            curr_function_names = keys(current_funcs) ;

            _debug("Source files: %O", source_files) ;

            foreach(curr_function_name in curr_function_names) {
                mapping func ;
                string func_md ;

                func = current_funcs[curr_function_name] ;
                func_md = generate_function_markdown(func) ;

                if(func_md) {
                    doc_content += sprintf("## %s\n\n", curr_function_name) ;
                    doc_content += func_md ;
                    doc_content += "\n" ;
                }
            }

            assure_file(dest_file) ;
            write_file(dest_file, doc_content, 1) ;
            doc_content = "" ;
        }

        assure_file(append(doc_root, function_type+".md")) ;
        // write_file(append(doc_root, ), home_content, 1) ;
        home_content = "" ;
    }

    writing = false ;
}

private nomask string dash_wrap(string str, int width) {
    int dash_pos, space_pos ;

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
}

public nomask int check_running() {
    return scanning > 0 || writing == true ;
}
