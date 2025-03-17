/**
 * @file /std/object/id.c
 * @description Manages object identifiers and adjectives for item recognition
 *              and command parsing.
 *
 * @created 2024-01-31 - Gesslar
 * @last_modified 2025-03-16 - GitHub Copilot
 *
 * @history
 * 2024-01-31 - Gesslar - Created
 * 2025-03-16 - GitHub Copilot - Added documentation
 */

#include <id.h>
#include <object.h>

private nosave string *_ids = ({});
private nosave string *_plural_ids = ({});
private nosave string *_adj = ({});

/**
 * Sets the primary identifiers for this object, replacing any existing IDs.
 *
 * @param {string|string*} str - Single ID or array of IDs
 * @errors If argument is not a string or array of strings
 */
void set_id(mixed str) {
  if(stringp(str))
    str = ({ str });

  if(!pointerp(str))
    error("Bad argument 1 to set_id().");

  if(!uniform_array(str, T_STRING))
    error("Bad argument 1 to set_id().");


  _ids = distinct_array(str);
}

/**
 * Sets the adjectives for this object, replacing any existing adjectives.
 *
 * These adjectives can be combined with IDs for identification.
 *
 * @param {string|string*} str - Single adjective or array of adjectives
 * @errors If argument is not a string or array of strings
 */
void set_adj(mixed str) {
  if(stringp(str))
    str = ({ str });

  if(!pointerp(str))
    error("Bad argument 1 to set_adj().");

  if(!uniform_array(str, T_STRING))
    error("Bad argument 1 to set_adj().");

  _adj = distinct_array(str);
}

/**
 * Adds additional identifiers to this object.
 *
 * @param {string|string*} str - Single ID or array of IDs to add
 * @errors If argument is not a string or array of strings
 */
void add_id(mixed str) {
  if(stringp(str))
    str = ({ str });

  if(!pointerp(str))
    error("Bad argument 1 to add_id().");

  if(!uniform_array(str, T_STRING))
    error("Bad argument 1 to add_id().");

  _ids = distinct_array(_ids + str);
}

/**
 * Adds additional adjectives to this object.
 *
 * @param {string|string*} str - Single adjective or array of adjectives to add
 * @errors If argument is not a string or array of strings
 */
void add_adj(mixed str) {
  if(stringp(str))
    str = ({ str });

  if(!pointerp(str))
    error("Bad argument 1 to add_adj().");

  if(!uniform_array(str, T_STRING))
    error("Bad argument 1 to add_adj().");

  _adj = distinct_array(_adj + str);
}

/**
 * Removes identifiers from this object.
 *
 * @param {string|string*} str - Single ID or array of IDs to remove
 * @errors If argument is not a string or array of strings
 */
void remove_id(mixed str) {
  if(stringp(str))
    str = ({ str });

  if(!pointerp(str))
    error("Bad argument 1 to remove_id().");

  if(!uniform_array(str, T_STRING))
    error("Bad argument 1 to remove_id().");

  _ids = distinct_array(_ids - str);
}

/**
 * Removes adjectives from this object.
 *
 * @param {string|string*} str - Single adjective or array of adjectives to remove
 * @errors If argument is not a string or array of strings
 */
void remove_adj(mixed str) {
  if(stringp(str))
    str = ({ str });

  if(!pointerp(str))
    error("Bad argument 1 to remove_adj().");

  if(!uniform_array(str, T_STRING))
    error("Bad argument 1 to remove_adj().");

  _adj = distinct_array(_adj - str);
}

/**
 * Returns all identifiers for this object.
 *
 * @returns {string*} Array of identifiers
 */
string *query_ids() {
  return _ids;
}

/**
 * Returns all plural identifiers for this object.
 *
 * @returns {string*} Array of plural identifiers
 */
string *query_plural_ids() {
  return _plural_ids;
}

/**
 * Returns all adjectives for this object.
 *
 * @returns {string*} Array of adjectives
 */
string *query_adjs() {
  return _adj;
}

/**
 * Driver apply to checks if a string matches one of this object's
 * identifiers.
 *
 * @param {string} arg - The string to check against identifiers
 * @returns {int} 1 if the string matches, 0 if not
 */
int id(string arg) {
  if(!arg || !stringp(arg))
    return 0;

  if(sizeof(_ids) < 1)
    _ids = ({query_name()});

  if(member_array(arg, _ids) != -1)
    return 1;

  return 0;
}

/**
 * Regenerates the full set of identifiers.
 *
 * Includes plurals and adjective combinations in the identifier list.
 * Called when identifiers or adjectives change.
 */
void rehash_ids() {
  string *tmp = ({});
  string _name;

  _name = query_name();

  tmp = ({ _name });
  _plural_ids = map(_ids, (: pluralize :));
  tmp += _ids + _plural_ids;

  foreach(string id in _ids)
    tmp += map(_adj, (: $1 + " " + $2 :), id);

  tmp += ({ _name });

  _ids = distinct_array(tmp);
}
