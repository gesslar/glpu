/**
 * @file /adm/simul_efun/english.c
 *
 * English language processing simul-efuns for text manipulation and grammar.
 * Provides functions for capitalization, articles, and pronoun generation
 * based on gender and grammar rules.
 *
 * @created Unknown
 * @last_modified 2024-03-11
 */

#include <simul_efun.h>

/**
 * Capitalizes the first letter of each word in a string.
 *
 * @param {string} str - Input string to capitalize
 * @returns {string} String with each word capitalized
 * @example
 * cap_words("hello world");  // Returns "Hello World"
 */
string cap_words(string str) {
  string *words;

  words = explode(str, " ");
  words = map(words, (: capitalize :));

  return implode(words, " ");
}

/**
 * List of words that typically aren't capitalized in titles.
 *
 * @type {string*}
 */
private nosave string *insignificant_words = ({
  "the", "of", "and", "in", "on", "at", "to", "for", "with", "from", "by",
  "as", "or", "is", "a", "an", "but", "nor", "yet", "so", "if", "then",
  "else", "when", "where", "why", "how" });

/**
 * Capitalizes significant words in a string, treating it as a title.
 *
 * Follows standard English title capitalization rules where major words
 * are capitalized but articles, conjunctions, and prepositions are not,
 * unless they start the title.
 *
 * @param {string} str - Text to capitalize
 * @param {int} [title=0] - Whether to force capitalize first word
 * @returns {string} Text with appropriate words capitalized
 * @example
 * cap_significant_words("the tale of two cities", 1);
 * // Returns "The Tale of Two Cities"
 */
varargs string cap_significant_words(string str, int title) {
  string *words;

  words = explode(str, " ");
  words = map(words, (:
    member_array($1, insignificant_words) == -1 ? capitalize($1) : $1
  :));

  str = implode(words, " ");

  if(title)
    str = capitalize(str);

  return str;
}

/**
 * Creates the possessive form of a noun.
 *
 * @param {string | object STD_ITEM} ob - Noun or object with query_name()
 * @returns {string} Possessive form ("'s" or "'" depending on ending)
 * @example
 * possessive_noun("James");  // Returns "James'"
 * possessive_noun("cat");    // Returns "cat's"
 */
string possessive_noun(mixed ob) {
  if(objectp(ob))
    ob = ob->query_name();

  if(!stringp(ob))
    return "its";

  if(ob[<1] == 's')
    return ob + "'";

  else
    return ob + "'s";
}

/**
 * Creates the possessive form of a proper noun.
 *
 * Always adds "'s" regardless of ending, following proper noun rules.
 *
 * @param {string|object STD_ITEM} ob - Proper noun or object with query_name()
 * @returns {string} Possessive form with "'s"
 * @example
 * possessive_proper_noun("James");  // Returns "James's"
 */
string possessive_proper_noun(mixed ob) {
  if(objectp(ob))
    ob = ob->query_name();

  if(!stringp(ob))
    return "its";

  return ob + "'s";
}

/**
 * Gets the possessive pronoun for a gender.
 *
 * @param {string|object STD_BODY} ob - Gender string or object with query_gender()
 * @returns {string} Possessive pronoun (his/hers/its/theirs)
 * @example
 * possessive_pronoun("female");  // Returns "hers"
 */
string possessive_pronoun(mixed ob) {
  if(objectp(ob))
    ob = ob->query_gender() || "neuter";

  if(!stringp(ob))
    return "its";

  switch(ob) {
    case "male" : return "his";
    case "female" : return "hers";
    case "other" : return "theirs";
    case "none" : "its";
    default: return "its";
  }
}

/**
 * Gets the possessive adjective for a gender.
 *
 * @param {string|object STD_BODY} ob - Gender string or object with query_gender()
 * @returns {string} Possessive adjective (his/her/its/their)
 * @example
 * possessive("female");  // Returns "her"
 */
string possessive(mixed ob) {
  if(objectp(ob))
    ob = ob->query_gender() || "neuter";

  if(!stringp(ob))
    return "its";

  switch(ob) {
    case "male" : return "his";
    case "female" : return "her";
    case "other" : return "their";
    case "none" : "its";
    default: return "its";
  }
}

/**
 * Gets the reflexive pronoun for a gender.
 *
 * @param {string|object STD_BODY} ob - Gender string or object with query_gender()
 * @returns {string} Reflexive pronoun (himself/herself/itself/themself)
 * @example
 * reflexive("female");  // Returns "herself"
 */
string reflexive(mixed ob) {
  if(objectp(ob))
    ob = ob->query_gender() || "neuter";

  if(!stringp(ob))
    return "itself";

  switch(ob) {
    case "male" : return "himself";
    case "female" : return "herself";
    case "other" : return "themself";
    case "none" : "itself";
    default: return "itself";
  }
}

/**
 * Gets the objective pronoun for a gender.
 *
 * @param {string|object} ob - Gender string or object with query_gender()
 * @returns {string} Objective pronoun (him/her/it/them)
 * @example
 * objective("female");  // Returns "her"
 */
string objective(mixed ob) {
  if(objectp(ob))
    ob = ob->query_gender() || "neuter";

  if(!stringp(ob))
    return "it";

  switch(ob) {
    case "male" : return "him";
    case "female" : return "her";
    case "other" : return "them";
    case "none" : "it";
    default: return "it";
  }
}

/**
 * Gets the subjective pronoun for a gender.
 *
 * @param {string|object} ob - Gender string or object with query_gender()
 * @returns {string} Subjective pronoun (he/she/it/they)
 * @example
 * subjective("female");  // Returns "she"
 */
string subjective(mixed ob) {
  if(objectp(ob))
    ob = ob->query_gender() || "neuter";

  if(!stringp(ob))
    return "it";

  switch(ob) {
    case "male" : return "he";
    case "female" : return "she";
    case "other" : return "they";
    case "none" : "it";
    default: return "it";
  }
}

/**
 * Determines the appropriate article for a word.
 *
 * @param {string} str - Word to check
 * @param {int} [definite=0] - Use "the" instead of "a/an"
 * @returns {string} Appropriate article (the/a/an)
 * @example
 * article("elephant");     // Returns "an"
 * article("bear", 1);      // Returns "the"
 */
varargs string article(string str, int definite) {
  if(!stringp(str))
    return "";

  if(definite)
    return "the";

  else
    if(of(str[0], ({ 'a', 'e', 'i', 'o', 'u' })))
      return "an";
    else
      return "a";
}

/**
 * Adds appropriate article to start of string.
 *
 * @param {string} str - String to add article to
 * @param {int} [definite=0] - Use "the" instead of "a/an"
 * @returns {string} String with article added
 * @example
 * add_article("elephant");     // Returns "an elephant"
 * add_article("bear", 1);      // Returns "the bear"
 */
varargs string add_article(string str, int definite) {
  string art = article(str, definite);

  if(!strlen(art))
    return str;

  return art + " " + str;
}

/**
 * Removes leading article from string if present.
 *
 * @param {string} str - String to process
 * @returns {string} String with any leading article removed
 * @example
 * remove_article("the bear");  // Returns "bear"
 * remove_article("an apple");  // Returns "apple"
 */
string remove_article(string str) {
  if(!stringp(str))
    return "";

  if(pcre_match(lower_case(str), "^(the|a|an) "))
    return str[strsrch(str, " ") + 1..];

  return str;
}
