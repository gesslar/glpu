/**
 * @file /cmds/verb/std/open.c
 *
 * The open command.
 *
 * @created 2025-03-18 - Gesslar
 * @last_modified 2025-03-18 - Gesslar
 *
 * @history
 * 2025-03-18 - Gesslar - Created
 */

inherit STD_VERB;

private string resolve_direction(string direction);

void setup() {
  add_rules(({"", "OBJ", "OBJ with OBJ"}), ({}));
}

mixed can_unlock() { return "Unlock what?"; }
mixed can_unlock_obj() { return "Unlock it with what?"; }

/**
 *
 * @param {STD_CONTAINER} container The container to be unlocked.
 * @param {OBJ_KEY} key The key to unlock the container.
 * @param {string} arg1 The container string supplied by the user.
 * @param {string} arg2 The key string supplied by the user.
 */
mixed can_unlock_obj_with_obj(object container, object key, string arg1, string arg2) {
  return 1;
}

/**
 *
 * @param {STD_CONTAINER} container The container to be unlocked.
 * @param {OBJ_KEY} key The key to unlock the container.
 * @param {string} arg1 The container string supplied by the user.
 * @param {string} arg2 The key string supplied by the user.
 */
mixed do_unlock_obj_with_obj(object container, object key, string arg1, string arg2) {
  container->set_locked(false);

  if(!container->is_locked()) {
    this_body()->simple_action("$N $vunlock the $o with a $o1.", container, key);
    return 1;
  }

  return "#You were unable to open the lock for some reason.";
}
