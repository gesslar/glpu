/* Do not remove the headers from this file! see /USAGE for more info. */

/**
 * verb_ob.c
 *
 * This is a standard verb handler.  It handles adding and removing
 * verb rules (to/from the parser efuns).
*/

#include <verbs.h>

inherit STD_OBJECT;

void mudlib_setup() {

}

private nosave string verb = query_file_name();
int flags = NEED_TO_SEE | NEED_TO_BE_ALIVE | NEED_TO_THINK;

protected varargs void add_rules(mixed *rules, mixed *syns) {
  parse_init();

  foreach(string rule in rules) {
    parse_add_rule(verb, rule);
    if(syns)
      foreach(string syn in syns)
        parse_add_synonym(syn, verb, rule);
  }
}

protected void set_flags(int f) {
  flags = f;
}

protected void add_flag(int f) {
  flags |= f;
}

protected void clear_flag(int f) {
  flags &= ~f;
}

string refer_to_object(/** @type {STD_ITEM} */ object ob) {
   // In the future, this should be smarter.  Ideally it would generate
   // something unique like 'my first sword' or 'the third sword on the table'
   return ob->query_real_name();
}

mixed try_to_acquire(/** @type {STD_ITEM} */ object ob) {
  string out;

  /**
  * for things that can be used in certain conditions without
  * actually being in the player's inventory
  */
  if(ob->always_usable())
    return 1;

  if(environment(ob) == this_body())
    return 1;

  out = "(Taking " + trim(ob->the_short());
  if(!environment(ob)) {
    out += " first).\n";
    write(out);
    write("What a quaint idea.\n");
    return 0;
  }

  if(environment(ob) != environment(this_body()))
    out += " from " + environment(ob)->the_short() + " first).";
  else
    out += " first)";

  write(out);
  this_body()->do_game_command("get " + refer_to_object(ob));

  return environment(ob) == this_body();
}

mixed check_ghost() {
  if(this_body()->query_ghost())
    return "But you're a ghost!\n";

  return 1;
}

mixed check_vision() {
  if(environment(this_body())->query_light())
    return 1;

  if(environment(this_body())->parent_environment_accessible())
    if(environment(environment(this_body()))->query_light())
      return 1;

  return "You can't see a thing!\n";
}

mixed check_condition() {
  mixed tmp;

  if(tmp = this_body()->check_condition(0))
    return tmp;

   return 1;
}

/* All(most) can_* functions should call this */
mixed default_checks() {
  mixed tmp;

  if((flags & NEED_TO_SEE) &&(tmp = check_vision()) != 1)
    return tmp;

  if((flags & NEED_TO_BE_ALIVE) && (tmp = check_ghost()) != 1)
    return tmp;

  // This checks stunned, asleep
  if((flags & NEED_TO_THINK))
    return check_condition();

  return 1;
}

void handle_obs(mixed *info, function callback, mixed extra...) {
  foreach(mixed ob in info) {
    if(stringp(ob)) {
      tell(ob);
    } else {
      //      write(ob->short() + ": ");
      evaluate(callback, ob, extra...);
    }
  }
}

/* we defined the rule, so assume by default we allow it */
mixed can_verb_rule(string verb, string rule) {
  return default_checks();
}

int do_verb_one(string verb, object ob) {
  if((flags & TRY_TO_ACQUIRE) && !try_to_acquire(ob)) {
    //    write("You can't get " + ob->the_short() +
    //        " in order to " + verb + " it.\n");
    return 0;
  }

  if(function_exists("do_" + verb, ob)) {
    call_other(ob, "do_" + verb);

    return 1;
  } else {
    // Safety net in case the object has no do_ function
    tell("Trying to " + verb + " " + ob->the_short() + " has no effect.\n");
    return 0;
  }
}

/* default behavior for OBJ rules */
void do_verb_obj(string verb, object ob) {
  if(do_verb_one(verb, ob))
    this_body()->simple_action("$N $v" + verb + " the $o.", ob);
}

/* default behavior for OBS rules */
void do_verb_obs(string verb, object *obs) {
  object *success =({});

  foreach(mixed ob in obs)
    if(objectp(ob))
      if(do_verb_one(verb, ob))
        success +=({ob});

  if(sizeof(success))
    this_body()->simple_action("$N $v" + verb + " the $o.", success);
}

// default behaviour for "" rules
// Use this where you want to prompt player to use an object with verb
void do_verb(string verb) {
  tell(sprintf("You can't just %s - you need to %s something.\n", verb, verb));
}
