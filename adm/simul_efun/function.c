#include "/adm/obj/simul_efun.h"
#include <function.h>

int valid_function(mixed f) {
    int fp ;

    fp = functionp(f) ;

    if(fp) {
        return !(fp & FP_OWNER_DESTED) ;
    }

    return false ;
}

varargs string call_trace(int colour) {
    string res;
    int i, n;
    object *objects;
    string *programs;
    string *functions;
    string *origins ;
    string *lines ;

    res = "";
    programs = call_stack(0);
    objects = call_stack(1);
    functions = call_stack(2);
    origins = call_stack(3) ;
    lines = call_stack(4) ;

    // now fix up the lines
    lines = map(lines, function(string line) {
        int num ;
        sscanf(line, "%*s:%d", num) ;
        return sprintf("%d", num) ;
    }) ;

    n = sizeof(programs);
    // We don't want to include the call_trace() function itself
    while(n-- > 1) {
        i = n ;
        res += sprintf("[\e<0039>%O\e<res>] \e<0119>%s\e<res>:\e<0206>%s\e<res>::\e<0208>%s\e<res>() (\e<0172>%s\e<res>)\n",
            objects[i],
            programs[i],
            lines[i],
            functions[i],
            origins[i]
        );
    }

    if(!colour) res = no_ansi(res) ;
    return res;
}

mixed *assemble_call_back(mixed arg...) {
    int sz ;

    if(!pointerp(arg))
        error("ERROR: Invalid argument passed to assemble_call_back().") ;

    sz = sizeof(arg) ;
    if(!sz)
        error("ERROR: No arguments passed to assemble_call_back().") ;

    if(objectp(arg[0])) {
        object ob ;
        string fun ;

        if(sz < 2)
            error("ERROR: No function passed to assemble_call_back().") ;

        ob = arg[0] ;
        fun = arg[1] ;
        if(sz > 2)
            arg = arg[2] ;
        else
            arg = ({}) ;

        if(!function_exists(fun, ob))
            error("ERROR: Function does not exist in object passed to assemble_call_back().") ;

        return ({ ob, fun, arg... }) ;
    } else if(valid_function(arg[0])) {
        if(sz > 1)
            return ({ arg[0], arg[1..]... }) ;
        else
            return ({ arg[0], ({})... }) ;
    }

    error("ERROR: Invalid function passed to assemble_call_back().") ;
}

mixed call_back(mixed cb, mixed new_arg...) {
    int sz ;
    mixed fun ;
    mixed final_arg = ({}) ;

    if(!pointerp(cb))
        error("ERROR: Invalid argument passed to call_back().") ;

    sz = sizeof(cb) ;
    if(!sz)
        error("ERROR: No arguments passed to call_back().") ;

    if(objectp(cb[0])) {
        object cb_ob = cb[0] ;
        string cb_fun ;
        mixed *curr ;

        if(sz < 2)
            error("ERROR: No function passed to call_back().") ;

        cb_fun = cb[1] ;

        if(sz > 2)
            curr = cb[2..] ;
        else
            curr = ({}) ;

        if(!function_exists(cb_fun, cb_ob))
            error("ERROR: Function does not exist in object passed to call_back().") ;

        final_arg = ({ new_arg... }) + curr ;

        fun = (: call_other, cb_ob, cb_fun :);
    } else if(valid_function(cb[0])) {
        function cb_fun = cb[0] ;
        mixed *curr ;

        if(sz > 1)
            curr = cb[1..] ;
        else
            curr = ({}) ;

        fun = cb_fun ;
        final_arg = ({ new_arg... }) + curr ;
    } else
        error("ERROR: Invalid function passed to call_back(): "+identify(cb)) ;

    return catch((*fun)(final_arg...)) ;
}
