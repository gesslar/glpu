/**
 * @file /cmds/verb/std/eat.c
 *
 * The eat command.
 *
 * @created 2025-03-18 - Gesslar
 * @last_modified 2025-03-18 - Gesslar
 *
 * @history
 * 2025-03-18 - Gesslar - Created
 */

inherit STD_VERB;

mixed can_eat() {
  return "Eat what?";
}

void do_eat_obj(object ob) {
  ob->consume(this_body());
}

void setup() {
  add_rules(({"", "OBJ"}), ({}));
}
