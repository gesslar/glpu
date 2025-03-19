/**
 * @file /cmds/verb/std/nibble.c
 *
 * A nibble command.
 *
 * @created 2025-03-18 - Gesslar
 * @last_modified 2025-03-18 - Gesslar
 *
 * @history
 * 2025-03-18 - Gesslar - Created
 */

inherit STD_VERB;

void setup() {
  add_rules(({"", "OBJ"}), ({"bite"}));
}

mixed can_nibble() {
  return "Nibble on what?";
}

void do_nibble_obj(object ob) {
  ob->nibble(this_body());
}
