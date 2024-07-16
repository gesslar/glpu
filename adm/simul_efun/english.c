#include "/adm/obj/simul_efun.h"

/**
 * @simul_efun cap_words
 * @description Capitalizes the first letter of each word in a string.
 * @param {string} str - The string to capitalize.
 * @returns {string} - The capitalized string.
 */
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

/**
 * @simul_efun cap_significant_words
 * @description Capitalizes significant words in a string, ignoring certain
 *              insignificant words. Optionally capitalizes the first word
 *              as a title.
 * @param {string} str - The string to capitalize.
 * @param {int} [title=0] - Whether to capitalize the first word as a title.
 * @returns {string} - The string with significant words capitalized.
 */
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

/**
 * @simul_efun possessive_noun
 * @description Returns the possessive form of a noun. If the noun ends with 's',
 *              it adds an apostrophe; otherwise, it adds 's.
 * @param {mixed} ob - The object or string to convert to possessive form.
 * @returns {string} - The possessive form of the noun.
 */
string possessive_noun(mixed ob) {
    if(objectp(ob)) ob = ob->query_name() ;
    if(!stringp(ob)) return "its" ;

    if(ob[<1] == 's') return ob + "'" ;
    else return ob + "'s" ;
}

/**
 * @simul_efun possessive_pronoun
 * @description Returns the possessive pronoun corresponding to the object's
 *              gender. Defaults to "its" for non-string or unknown gender.
 * @param {mixed} ob - The object or gender string to convert.
 * @returns {string} - The possessive pronoun.
 */
string possessive_pronoun(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "its" ;

    switch(ob) {
        case "male" : return "his" ;
        case "female" : return "hers" ;
        default: return "its" ;
    }

/**
 * @simul_efun possessive
 * @description Returns the possessive adjective corresponding to the object's
 *              gender. Defaults to "its" for non-string or unknown gender.
 * @param {mixed} ob - The object or gender string to convert.
 * @returns {string} - The possessive adjective.
 */}

string possessive(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "its" ;

    switch(ob) {
        case "male" : return "his" ;
        case "female" : return "her" ;
        default: return "its" ;
    }
}

/**
 * @simul_efun reflexive
 * @description Returns the reflexive pronoun corresponding to the object's
 *              gender. Defaults to "itself" for non-string or unknown gender.
 * @param {mixed} ob - The object or gender string to convert.
 * @returns {string} - The reflexive pronoun.
 */
string reflexive(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "itself" ;

    switch(ob) {
        case "male" : return "himself" ;
        case "female" : return "herself" ;
        default: return "itself" ;
    }
}

/**
 * @simul_efun objective
 * @description Returns the objective pronoun corresponding to the object's
 *              gender. Defaults to "it" for non-string or unknown gender.
 * @param {mixed} ob - The object or gender string to convert.
 * @returns {string} - The objective pronoun.
 */
string objective(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "it" ;

    switch(ob) {
        case "male" : return "him" ;
        case "female" : return "her" ;
        default: return "it" ;
    }
}

/**
 * @simul_efun subjective
 * @description Returns the subjective pronoun corresponding to the object's
 *              gender. Defaults to "it" for non-string or unknown gender.
 * @param {mixed} ob - The object or gender string to convert.
 * @returns {string} - The subjective pronoun.
 */
string subjective(mixed ob) {
    if(objectp(ob)) ob = ob->query_gender() || "neuter" ;
    if(!stringp(ob)) return "it" ;

    switch(ob) {
        case "male" : return "he" ;
        case "female" : return "she" ;
        default: return "it" ;
    }
}
