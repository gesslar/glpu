/**
 * @file /obj/mudlib/key.c
 *
 * Base for keys.
 *
 * @created 2025-03-19 - Gesslar
 * @last_modified 2025-03-19 - Gesslar
 *
 * @history
 * 2025-03-19 - Gesslar - Created
 */

inherit STD_ITEM;

public void set_key_id(string str);
public string query_key_id();

private nosave string _key_id;

public void set_key_id(string str) {
  assert_arg(stringp(str) && truthy(str), 1, "Invalid key id.");

  if(stringp(_key_id))
    remove_id("#"+_key_id);

  _key_id = str;

  add_id("#"+_key_id);
}

public string query_key_id() {
  return _key_id;
}

/**
 *
 * @param {STD_CONTAINER} container The container to be unlocked.
 * @param {OBJ_KEY} key The key to unlock the container.
 * @param {string} arg1 The container string supplied by the user.
 * @param {string} arg2 The key string supplied by the user.
 */
mixed indirect_unlock_obj_with_obj(object container, object key, string arg1, string arg2) {
  if(environment() != previous_object())
    return "#You must be holding something to unlock with it.";

  if(container->query_key_id() != key->query_key_id())
    return "#That key does not turn this lock.";

  return 1;
}

public int is_key() { return 1; }
