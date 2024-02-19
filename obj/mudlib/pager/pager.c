/* pager.c

 Tacitus @ LPUniversity
 04-NOV-05
 Standard file pager

 Last edited on August 15th by Parthenon

*/

inherit STD_OBJECT ;

#define DEF_LINESPERCYCLE 40

string *exploded;
function cb;
int current_line = 0;
int total_lines;
int lines_per_cycle = DEF_LINESPERCYCLE;
int my_lines_per_cycle;
int ansi = 1;

int page(string data, string title);
int page_more();
int prompt(string arg);

varargs int page(string data, string title, function callback, int no_ansi) {
    if(!stringp(data)) return 0;
    if(stringp(title)) write("*===| Paging: " + title + " |===*\n\n");
    if(valid_function(callback)) cb = callback;
    if(no_ansi) ansi = 0;

    current_line = 0;

    if(!this_player()->query_env("morelines"))
        my_lines_per_cycle = DEF_LINESPERCYCLE;
    else
        my_lines_per_cycle = to_int(this_player()->query_env("morelines"));

    lines_per_cycle = my_lines_per_cycle - 1;

    data = replace_string(data, "\\e", "\e") ;
    exploded = explode(data, "\n");
    total_lines = sizeof(exploded);
    page_more();
}

int page_more() {
    int start, end;
    int msg_type ;

    start = current_line + 1;

    if(!ansi) msg_type = msg_type | NO_ANSI ;
    for(;current_line < total_lines && current_line < lines_per_cycle; current_line++)
        tell(this_player(), exploded[current_line], msg_type);

    end = current_line;

    if(total_lines >= my_lines_per_cycle) {
        switch(this_player()->query_env("page_display")) {
            case "percent" :
                write("\n-=-= [" + percent_of(current_line, total_lines) + "%] =-=-\n");
                break;
            case "lines" :
            default :
                write("\n-=-= [" + start + "-" + end + " of " + total_lines + "] =-=-\n");
                break;
        }
    }

    if(current_line < total_lines) {
        input_to("prompt");
        return 1;
    }

    write("\n");
    evaluate(cb);
    remove() ;
    return 1;
}


int prompt(string arg) {
    switch(arg) {
        case "help" :
        case "h" : {
            write("\n\t -= File Pager Help =-\n\n");
            write("  h, help     : Display this help\n");
            write("  q, Q        : Quit\n");
            write("  n           : Goto line n\n");
            write("  s, '        : Start over\n\n");
            input_to("prompt");
            return 1;
        }
        case "q" :
        case "Q" : evaluate(cb); remove() ; return 1;
        case "\r" :
        case "\n" :
        case " " :
        case "" :
        case "\n\r" :
        {
            lines_per_cycle += my_lines_per_cycle - 1;
            break;
        }
        case "s" :
        case "'" :
        {
            lines_per_cycle = my_lines_per_cycle;
            current_line = 0;
            break;
        }
        default :
        {
            if(intp(to_int(arg)))
            {
                current_line = to_int(arg);
                lines_per_cycle = to_int(arg) + my_lines_per_cycle;
            }
            break;
        }
    }

    page_more();
}

void reset() {
    if(!environment()) remove() ;
    if(!in_input(environment())) remove() ;
}
