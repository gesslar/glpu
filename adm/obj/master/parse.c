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

string parser_gen_pos(object ob, int num, int indirect) {
  object env = environment(ob);
  string env_str;
  string str = "";

  if(env == 0)
    return 0;

  if(num > 1) {
    str += number_word(num) + " ";

    if(!ob->query_plural())
      str += pluralize(ob->short());
    else
      str += ob->short();
  } else {
    str += ob->short();
  }

  if(env == this_body()) {
    return "your " + str;
  } else if(env == environment(this_body())) {
    if(indirect)
      return "the " + str;
    else
      return "the " + str + " on the ground";
  } else if(env->is_living()) {
      return env->query_name() + "'s " + str;
  } else {
    string rel = env->query_relation(ob);
    string loc;

    if(environment(env))
      loc = parser_gen_pos(env, 1, 1);
    else
      loc = env->the_short();
    return "the " + str + " " + rel + " " + loc;
  }
}

string parser_error_message(int kind, object ob, mixed arg, int flag) {
  string ret;

  if(ob)
    ret = ob->short() + ": ";
  else
    ret = "";

  switch(kind) {
    case ERR_IS_NOT:
      return ret + "There is no such " + arg + " here.\n";
      break;
    case ERR_NOT_LIVING:
      if(flag)
        return ret + "None of the " + pluralize(arg) + " are alive.\n";

      return ret + "The " + arg + " isn't alive.\n";
      break;
    case ERR_NOT_ACCESSIBLE:
      if(flag)
         return ret + "You can't reach them.\n";
      else
         return ret + "You can't reach it.\n";
      break;
    case ERR_AMBIG: {
        mixed *descs = unique_array(arg, (:parser_gen_pos($1, 1, 0):));
        string str;

        if(sizeof(descs) == 1)
          str = ret + "Which of ";
        else
          str = ret + "Do you mean ";

        for(int i = 0; i < sizeof(descs); i++) {
          str += parser_gen_pos(descs[i][0], sizeof(descs[i]), 0);

          if(i < sizeof(descs) - 2)
            str += ", ";
          else if(i == sizeof(descs) - 2)
            str += " or ";
        }

        if(sizeof(descs) == 1)
          return str + " do you mean?\n";
        else
          return str + "?\n";
      }
      break;
    case ERR_ORDINAL:
      if(arg > 1)
        return ret + "There are only " + arg + " of them.\n";
      else
        return ret + "There is only one of them.\n";
    case ERR_ALLOCATED:
      return ret + arg;
    case ERR_THERE_IS_NO:
      return ret + "There is no " + arg + " here.\n";
    case ERR_BAD_MULTIPLE:
      return ret + "You can't use more than one object at a time with that verb.\n";
  }
}
