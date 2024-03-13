#include "/adm/obj/simul_efun.h"

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
