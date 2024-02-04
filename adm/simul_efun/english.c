#include "/adm/obj/simul_efun.h"

string cap_words(string str) {
    string *words ;

    words = explode(str, " ") ;
    words = map(words, (: capitalize :)) ;

    return implode(words, " ") ;
}

private nosave string *insignificant_words = ({
    "the", "of", "and", "in", "on", "at", "to", "for", "with", "from", "by",
    "as", "or", "is", "a", "an", "but", "nor", "yet", "so", "if", "then",
    "else", "when", "where", "why", "how" }) ;

varargs string cap_significant_words(string str, int title) {
    string *words ;

    words = explode(str, " ") ;
    words = map(words, (:
        member_array($1, insignificant_words) == -1 ? capitalize($1) : $1
    :)) ;
    str = implode(words, " ") ;
    if(title) str = capitalize(str) ;

    return str ;
}
