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

string possessive_noun(mixed ob) {
    if(objectp(ob)) ob = ob->query_name() ;
    if(!stringp(ob)) return "its" ;

    if(ob[<1] == 's') return ob + "'" ;
    else return ob + "'s" ;
}

string possessive_pronoun(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "its" ;

    switch(ob) {
        case "male" : return "his" ;
        case "female" : return "hers" ;
        default: return "its" ;
    }
}

string possessive(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "its" ;

    switch(ob) {
        case "male" : return "his" ;
        case "female" : return "her" ;
        default: return "its" ;
    }
}

string reflexive(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "itself" ;

    switch(ob) {
        case "male" : return "himself" ;
        case "female" : return "herself" ;
        default: return "itself" ;
    }
}

string objective(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "it" ;

    switch(ob) {
        case "male" : return "him" ;
        case "female" : return "her" ;
        default: return "it" ;
    }
}

string subjective(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "it" ;

    switch(ob) {
        case "male" : return "he" ;
        case "female" : return "she" ;
        default: return "it" ;
    }
}
