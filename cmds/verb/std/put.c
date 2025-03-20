/**
 * @file /cmds/verb/std/put.c
 *
 * The put command for placing objects into containers.
 *
 * @created 2025-03-19 - Gesslar
 * @last_modified 2025-03-19 - Gesslar
 *
 * @history
 * 2025-03-19 - Gesslar - Created
 */

inherit STD_VERB ;

void setup() {
  add_rules(({"", "OBJ in OBJ"}), ({"OBJ into OBJ"})) ;
}

mixed can_put() {
  return "Put what where?" ;
}

mixed can_put_obj_in_obj(mixed args...) {return 1;}

/**
 * Puts an object into a container.
 *
 * @param {STD_ITEM} obj - The object to put
 * @param {STD_CONTAINER} container - The container to put the object into
 */
void do_put_obj_in_obj(object obj, object container, string arg1, string arg2) {
  if(container->add_inventory(obj))
    this_body()->simple_action("$N $vput a $o into a $o1.", obj, container) ;
  else
    this_body()->simple_action("$N $vtry to put a $o into a $o1, but fail.", obj, container) ;
}
