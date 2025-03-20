/**
 * @file /cmds/verb/std/sip.c
 *
 * The sip command.
 *
 * @created 2025-03-19 - Gesslar
 * @last_modified 2025-03-19 - Gesslar
 *
 * @history
 * 2025-03-19 - Gesslar - Created
 */

inherit STD_VERB ;

mixed can_sip() {
  return "Sip what?" ;
}

void do_sip_obj(object ob) {
  ob->sip(this_body()) ;
}

void setup() {
  add_rules(({"", "OBJ"}), ({"from"})) ;
}
