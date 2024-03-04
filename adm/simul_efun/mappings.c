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

mixed element_of_weighted(mapping m) {
    mixed *keys ;
    int *values ;
    int i ;
    int total ;
    int roll ;

    keys = keys(m) ;
    values = values(m) ;
    total = sum(values) ;

    roll = random(total) ;
    i = sizeof(values) ;
    while(i--) {
        if(roll < values[i])
            return keys[i] ;
        roll -= values[i] ;
    }

    return keys[0] ;
}
