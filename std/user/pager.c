/**
 * @file /std/user/pager.c
 * @description Pager object for displaying text to the user in a paginated
 *              format.
 *
 * @created 2024/07/23 - Gesslar
 * @last_modified 2024/07/23 - Gesslar
 *
 * @history
 * 2024/07/23 - Gesslar - Created
 */

#include <type.h>

// Functions from other objects
mixed query_env(string var);

// Functions
void continue_page(string *text, mixed *cb, int no_ansi, int lines, string display, int num, int curr) ;

varargs void page(mixed text, mixed *cb, int no_ansi) {
    int lines, sz ;
    string display ;

    if(nullp(text))
        error("Bad argument 1 to page().") ;

    if(stringp(text))
        text = explode(text, "\n");

    if(!pointerp(text))
        return 0 ;

    if(!uniform_array(T_STRING, text))
        return 0 ;

    lines = to_int(query_env("morelines")) || mud_config("MORELINES") ;
    display = query_env("page_display") || mud_config("PAGE_DISPLAY") ;

    if(no_ansi == 1)
        no_ansi = MSG_PROMPT | NO_ANSI ;
    else
        no_ansi = MSG_PROMPT ;

    sz = sizeof(text) ;
    continue_page(text, cb, no_ansi, lines, display, sz, 0) ;
}

void continue_page(string *text, mixed *cb, int no_ansi, int lines, string display, int num, int curr) {
    int i, j, sz ;
    string mess ;

    if(curr >= num) {
        if(nullp(cb)) {
            call_back(cb) ;
            return ;
        }
    }

    mess = implode(text[curr..curr + lines - 1], "\n") ;
    text = text[curr + lines..] ;
    curr += lines ;

    if(sizeof(text) > 0) {
        switch(display) {
            case "percent" :
                mess += sprintf("[%d%% - Press <Return> to continue, q to quit]", percent(curr, num)) ;
                break ;
            default:
                mess += sprintf("[%d/%d - Press <Return> to continue, q to quit]", curr, num) ;
                break ;
        }

        tell(this_object(), mess, no_ansi) ;
        input_to("continue_page", text, cb, no_ansi, lines, display, num, curr) ;
    } else {
        tell(this_object(), mess, no_ansi) ;
        if(!nullp(cb))
            call_back(cb) ;
    }
}
