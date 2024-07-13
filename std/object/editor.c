/**
 * @file /std/object/editor.c
 * @description Editor object that can be inherited to provide editing
                capabilities.
 *
 * @created 2024/07/07 - Gesslar
 * @last_modified 2024/07/07 - Gesslar
 *
 * @history
 * 2024/07/07 - Gesslar - Created
 */

private nomask begin_edit(object tp, string text, string file) ;

// Functions from other objects
int remove() ;

void edit(object tp, string source_file, mixed *callback) {
    string *lines;
    string text;
    string temp ;
    int i;

    if(!tp)
        error("ERROR: No valid player object to edit_file().");

    if(nullp(callback))
        error("ERROR: No valid callback to edit_file()." );

    text = "" ;

    if(stringp(source_file)) {
        if(!file_exists(source_file)) {
            error("ERROR: File does not exist: " + source_file);
        }

        if(file_size(source_file) > get_config(__MAX_READ_FILE_SIZE__))
            error("ERROR: File is too large to read: " + source_file);
        text = read_file(source_file);
    }

    if(sizeof(text))
        text = append(text, "\n");

    temp = temp_file(tp) ;

    tell(tp, text) ;
    tell(tp, "] ", MSG_PROMPT) ;

    input_to("parse_input", 0, tp, text, temp, callback) ;
}

void parse_input(string input, object tp, string text, string file, mixed *callback) {
    string *lines;
    string temp ;
    int i;

    if(!input) {
        this_player()->tell("Aborted.\n") ;
        remove() ;
        return ;
    }

    if(input == ".") {
        call_back(callback, text) ;
        remove() ;
        return ;
    }

    text += input + "\n" ;
    tell(tp, "] ", MSG_PROMPT) ;
    input_to("parse_input", 0, tp, text, file, callback) ;
}
