/**
 * @file /cmds/verb/std/drink.c
 *
 * The drink command.
 *
 * @created 2025-03-19 - Gesslar
 * @last_modified 2025-03-19 - Gesslar
 *
 * @history
 * 2025-03-19 - Gesslar - Created
 */

inherit STD_VERB ;

mixed can_drink() {
  return "Drink what?" ;
}

void do_drink_obj(object ob) {
  ob->drink(this_body()) ;
}

void setup() {
  add_rules(({"", "OBJ"}), ({})) ;
}
