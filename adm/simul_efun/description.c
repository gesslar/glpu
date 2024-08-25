#include <simul_efun.h>

/**
 * @simul_efun get_short
 * @description Returns the short description of an object, optionally
 *              including extra short descriptions in parentheses.
 * @param {object} ob - The object to get the short description of.
 * @param {int} [extras=1] - Whether to include extra short descriptions.
 *                           Defaults to 1 (include extras).
 * @returns {string} - The short description of the object, including any
 *                     extra short descriptions.
 */
varargs string get_short(object ob, int extras) {
    string result ;
    string *extra_shorts ;

    if(!ob)
        ob = previous_object() ;
    if(nullp(extras))
        extras = 1 ;

    result = ob->query_short() ;
    if(!result) return "" ;

    extras = !!extras ;
    if(extras == 1) {
        extra_shorts = ob->query_extra_shorts() || ({ }) ;
        extra_shorts = filter(extra_shorts, (: stringp :)) ;
        extra_shorts = filter(extra_shorts, (: $1 != "" :)) ;
        extra_shorts -= ({ 0 }) ;
        if(sizeof(extra_shorts)) {
            extra_shorts = map(extra_shorts, (: sprintf("(%s)", $1) :)) ;
            result += " " + implode(extra_shorts, " ") ;
        }
    }

    return result ;
}

/**
 * @simul_efun get_long
 * @description Returns the long description of an object, optionally
 *              including extra long descriptions.
 * @param {object} ob - The object to get the long description of.
 * @param {int} [extras=1] - Whether to include extra long descriptions.
 *                           Defaults to 1 (include extras).
 * @returns {string} - The long description of the object, including any
 *                     extra long descriptions.
 */
string get_long(object ob, int extras) {
    string result ;
    string *extra_longs, *parts ; ;

    if(!ob)
        ob = previous_object() ;
    if(nullp(extras))
        extras = 1 ;

    result = ob->query_long() ;
    if(!result) return "" ;

    // remove all double newlines
    parts = explode(result, "\n\n") ;
    parts = map(parts, function(string part) {
        while(part[<1] == '\n') part = part[0..<2] ;
        return part ;
    }) ;
    result = implode(parts, "\n\n") ;

    extras = !!extras ;
    if(extras == 1) {
        extra_longs = ob->query_extra_longs() || ({ }) ;
        extra_longs = filter(extra_longs, (: stringp :)) ;
        extra_longs = filter(extra_longs, (: $1 != "" :)) ;
        extra_longs -= ({ 0 }) ;
        if(sizeof(extra_longs)) {
            extra_longs = map(extra_longs, function(string extra_long) {
                while(extra_long[<1] == '\n') extra_long = extra_long[0..<2] ;
                return extra_long + "\n" ;
            }) ;

            result += "\n" + implode(extra_longs, "\n") ;
        }
    }

    while(result[<1] == '\n')
        result = result[0..<2] ;

    return result ;
}
