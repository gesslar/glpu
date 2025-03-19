/**
 * @file /adm/obj/master/parse.c
 *
 * Parser implementation stolen from LIMA and gently massaged into
 * something workable for Oxidus.
 *
 * Love you Tsdth! 🥰🥰🥰 Even if you indent with 3 spaces.
 *
 * @created 2025-03-16 - Gesslar
 * @last_modified 2025-03-16 - Gesslar
 *
 * @history
 * 2025-03-16 - Gesslar - Created
 */

#include <driver/parser_error.h>

/**
 * Default language functions used by parse_command()
 */
string *parse_command_id_list() {
  return({"one", "thing"});
}

string *parse_command_plural_id_list() {
  return({
    "ones",
    "stuff",
    "things",
  });
}

string *parse_command_adjective_id_list() {
  return({});
}

string *parse_command_prepos_list() {
  return({
    "in",           "from",         "on",         "under",        "behind",
    "beside",       "of",           "for",        "to",           "with",
    "at",           "off",          "out",        "down",         "up",
    "around",       "over",         "into",       "about",        "onto",
    "out of",       "aboard",       "above",      "against",      "alongside",
    "below",        "beneath",      "besides",    "by",           "inside",
    "outside",      "through",      "underneath", "upon",         "within",
    "without",      "alongside of", "back of",    "down from",    "inside of",
    "round about",  "near to",      "next to",    "over to",      "outside of",
    "up to",        "in front of",  "in back of", "on top of",    "off of",
    "astride",
  });
}

string parse_command_all_word() {
  return "all";
}

/* needs to be called when parse_command_users() changes */
void refresh_parse_info() {
  parse_refresh();
}

object *parse_command_users() {
  return users()->query_body();
}

string parser_error_message(int kind, object ob, mixed arg, int flag) {
  string thing;

  // Convert the object to its short description or use provided arg
  if(objectp(ob))
    thing = ob->short();
  else if(stringp(arg))
    thing = arg;
  else
    thing = "that";

  switch(kind) {
    case ERR_IS_NOT:
      return "There is no such " + arg + " here.\n";

    case ERR_NOT_LIVING:
      if(flag)
        return "None of the " + pluralize(arg) + " are alive.\n";
      return "The " + arg + " isn't alive.\n";

    case ERR_NOT_ACCESSIBLE:
      if(flag)
        return "You can't reach those.\n";
      return "You can't reach " + thing + ".\n";

    case ERR_AMBIG:
      if(pointerp(arg)) {
        string items = simple_list(map(arg, (: get_short :)), "or");
        return "Do you mean " + items + "?\n";
      }
      return "Which one do you mean?\n";

    case ERR_ORDINAL:
      if(arg > 1)
        return "There are only " + arg + " of them.\n";
      return "There is only one.\n";

    case ERR_ALLOCATED:
      return arg;

    case ERR_THERE_IS_NO:
      return "There is no " + arg + " here.\n";

    case ERR_BAD_MULTIPLE:
      return "You can't use more than one at a time.\n";

    default:
      return "That doesn't seem possible.\n";
  }
}
