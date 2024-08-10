#include <simul_efun.h>

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
 * @simul_efun possessive_proper_noun
 * @description Returns the possessive form of a proper noun. Applies 's to the
 *              end of the noun.
 * @param {mixed} ob - The object or string to convert to possessive form.
 * @returns {string} - The possessive form of the proper noun.
 */
string possessive_proper_noun(mixed ob) {
    if(objectp(ob)) ob = ob->query_name() ;
    if(!stringp(ob)) return "its" ;

    return ob + "'s" ;
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
        case "other" : return "theirs" ;
        case "none" : "its" ;
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
        case "other" : return "their" ;
        case "none" : "its" ;
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
        case "other" : return "themself" ;
        case "none" : "itself" ;
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
        case "other" : return "them" ;
        case "none" : "it" ;
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
        case "other" : return "they" ;
        case "none" : "it" ;
        default: return "it" ;
    }
}

/**
  * @simul_efun article
  * @description Returns the article corresponding to the noun. If definite
  *              is true, returns "the"; otherwise, returns "a" or "an"
  *              depending on the noun's initial letter.
  * @param {string} str - The noun to determine the article for.
  * @param {int} [definite=0] - Whether to return the definite article.
  * @returns {string} - The article.
 */
varargs string article(string str, int definite) {
    if(!stringp(str))
        return "" ;

    if(definite)
        return "the" ;

    else
        if(of(str[0], ({ 'a', 'e', 'i', 'o', 'u' })))
            return "an" ;
        else
            return "a" ;
}

/**
 * @simul_efun add_article
 * @description Adds an article to a string. If definite is true, adds "the";
 *              otherwise, adds "a" or "an" depending on the noun's initial
 *              letter.
 * @param {string} str - The string to add the article to.
 * @param {int} [definite=0] - Whether to add the definite article.
 * @returns {string} - The string with the article prepended.
 */
varargs string add_article(string str, int definite) {
    string art = article(str, definite) ;

    if(!strlen(art))
        return str ;

    return art + " " + str ;
}

/**
 * @simul_efun remove_article
 * @description Removes an article from a string. If the string begins with
 *              "the ", "a ", or "an ", removes the article.
 * @param {string} str - The string to remove the article from.
 * @returns {string} - The string with the article removed.
 */
string remove_article(string str) {
    int pos ;

    if(!stringp(str))
        return "" ;

    if(pcre_match(lower_case(str), "^(the|a|an) "))
        return str[strsrch(str, " ") + 1..] ;

    return str ;
}
