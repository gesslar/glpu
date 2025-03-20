/**
 * @file /cmds/verb/std/dispose.c
 *
 * The dispose command for getting rid of corpses.
 *
 * @created 2025-03-19 - Gesslar
 * @last_modified 2025-03-19 - Gesslar
 *
 * @history
 * 2025-03-19 - Gesslar - Created
 */

inherit STD_VERB;

mixed can_dispose() {
  return "Dispose of what?";
}

void do_dispose_obj(object ob) {
  this_body()->simple_action("Quickly and quietly, $n $vwork to dispose of the $o.", ob);
  ob->remove();
}

void setup() {
  add_rules(({"", "OBJ"}), ({}));
}
