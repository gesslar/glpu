#include "/adm/obj/simul_efun.h"

private string regex = "( \\/\\* sizeof\\(\\) == \\d+ \\*/)" ;

string pretty_map(mapping map) {
    string str = sprintf("%O\n", map) ;
    mixed *assoc ;
    string *elements ;
    int *matches ;
    string result ;
    int i ;

    assoc = pcre_assoc(str, ({ regex }), ({ 1 })) ;
    elements = assoc[0] ;
    matches = assoc[1] ;

    i = sizeof(matches) ;
    while(i--) {
        if(matches[i] == 1) {
            elements[i] = "" ;
        }
    }

    return implode(elements, "") ;
}
