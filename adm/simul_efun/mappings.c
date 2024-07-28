#include <simul_efun.h>

private string regex = "( \\/\\* sizeof\\(\\) == \\d+ \\*/)" ;

/**
 * @simul_efun pretty_map
 * @description Returns a formatted string representation of a mapping, removing
 *              any size annotations.
 * @param {mapping} map - The mapping to format.
 * @returns {string} - The formatted string representation of the mapping.
 */
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
