/**
 * @file /std/object/container.c
 * @description Container inheritable for objects that can hold other objects.
 *
 * @created 2024-07-23 - Gesslar
 * @last_modified 2025-03-16 - GitHub Copilot
 *
 * @history
 * 2024-07-23 - Gesslar - Created
 * 2025-03-16 - GitHub Copilot - Added documentation
 */

inherit __DIR__ "inventory";
inherit __DIR__ "contents";

private nosave int _ignore_capacity = 0;
private nosave int _ignore_mass = 0;
private nosave int _closeable = 0;
private nosave int _lockable = 0;
private nosave int _closed = 0;
private nosave int _locked = 0;
private nosave int _opaque = 1;

/**
 * Checks if the inventory of this object is accessible from the perspective
 * of another object.
 *
 * An inventory is considered accessible (within reach), if:
 *
 *  - it is not closed
 *  - it is in the same environment as this object (like a chest in a room)
 *  - it is in this object's inventory (like a player in a room)
 *
 * This function is merely about technicality. It only checks for closed
 * status and proximity. It doesn't check for any other conditions. You
 * will need to accommodate for those. For example, a living's access to
 * another living's inventory.
 *
 * A note about "it is not closed" condition. What this means is that
 * the container must be closed AND closeable. Containers that are not
 * closeable are considered to be open; therefore, it must pass both
 * conditions.
 *
 * @param {STD_ITEM} pov The object trying go access the inventory.
 * @returns {int} 1 if accessible, otherwise 0.
 */
varargs int is_content_accessible(object pov) {
  int result;
  object env, env_pov;

  if(is_closeable() && is_closed())
    return 0;

  if(!pov)
    return 1;

  // If we are open, anything outside can reach us,
  // and anything in the same environment as us can
  // reach us.
  env = environment();

  return objectp(present(pov, this_object())) ||
         objectp(present(pov, env));
}

int inventory_accessible() { return is_content_accessible(); }
int inventory_visible() { return is_content_accessible(); }

/**
 * Checks if an object can be received into this container.
 *
 * This function can be overridden by inheriting objects to add
 * restrictions on what objects can enter the container.
 *
 * @param {object} ob - The object attempting to enter
 * @returns {int} 1 if the object can be received, 0 if not
 */
int can_receive(object ob) {
  return 1;
}

/**
 * Called when an object is received by this container.
 *
 * This function can be overridden to implement special effects
 * when objects enter the container.
 *
 * @param {object} ob - The object that entered the container
 */
void receive_effect(object ob) {}

/**
 * Checks if an object can be released from this container.
 *
 * This function can be overridden to add restrictions on what
 * objects can leave the container.
 *
 * @param {object} ob - The object attempting to leave
 * @returns {int} 1 if the object can be released, 0 if not
 */
int can_release(object ob) {
  return 1;
}

/**
 * Called when an object is released from this container.
 *
 * This function can be overridden to implement special effects
 * when objects leave the container.
 *
 * @param {object} ob - The object that left the container
 */
void release_effect(object ob) {}

/**
 * Sets whether this container ignores capacity limits.
 *
 * @param {int} i - 1 to ignore capacity, 0 to enforce capacity limits
 */
void set_ignore_capacity(int i) {
  rehash_capacity();

  _ignore_capacity = !!i;
}

/**
 * Checks if this container ignores capacity limits.
 *
 * @returns {int} 1 if ignoring capacity, 0 if enforcing capacity limits
 */
int ignore_capacity() {
  return _ignore_capacity;
}

/**
 * Sets whether this container ignores mass limits.
 *
 * @param {int} i - 1 to ignore mass, 0 to enforce mass limits
 */
void set_ignore_mass(int i) {
  rehash_capacity();

  _ignore_mass = !!i;
}

/**
 * Checks if this container ignores mass limits.
 *
 * @returns {int} 1 if ignoring mass, 0 if enforcing mass limits
 */
int ignore_mass() {
  return _ignore_mass;
}

/**
 * Sets whether this container can be opened and closed.
 *
 * @param {int} i - 1 for closeable, 0 for not closeable
 */
void set_closeable(int i) {
  _closeable = !!i;
}

/**
 * Checks if this container can be opened and closed.
 *
 * @returns {int} 1 if closeable, 0 if not closeable
 */
int is_closeable() {
  return _closeable;
}

/**
 * Sets whether this container can be locked.
 *
 * @param {int} i - 1 for lockable, 0 for not lockable
 */
void set_lockable(int i) {
  _lockable = !!i;
}

/**
 * Checks if this container can be locked.
 *
 * @returns {int} 1 if lockable, 0 if not lockable
 */
int is_lockable() {
  return _lockable;
}

/**
 * Sets the closed state of this container.
 *
 * @param {int} i - 1 for closed, 0 for open
 */
void set_closed(int i) {
  _closed = !!i;
}

/**
 * Checks if this container is closed.
 *
 * @returns {int} 1 if closed, 0 if open
 */
int is_closed() {
  return _closed;
}

/**
 * Sets the locked state of this container.
 *
 * @param {int} i - 1 for locked, 0 for unlocked
 */
void set_locked(int i) {
  _locked = !!i;
}

/**
 * Checks if this container is locked.
 *
 * @returns {int} 1 if locked, 0 if unlocked
 */
int is_locked() {
  return _locked;
}

/**
 * Sets whether this container's contents are visible from outside when closed.
 *
 * @param {int} i - 1 for opaque (contents hidden), 0 for transparent
 */
void set_opaque(int i) {
  _opaque = !!i;
}

/**
 * Checks if this container's contents are visible from outside when closed.
 *
 * @returns {int} 1 if opaque (contents hidden), 0 if transparent
 */
int is_opaque() {
  return _opaque;
}

private nosave string _key_id;

public void set_key_id(string str) {
  assert_arg(stringp(str) && truthy(str), 1, "Invalid key id.");

  _key_id = str;
}

public string query_key_id() {
  return _key_id;
}

mixed indirect_put_obj_in_obj(object ob, object container, string arg1, string arg2) {
  if(!ob)
    return 1;

  if(ob == container)
    return "#You cannot put " + get_short(ob) + " in itself.";

  if(is_closeable() && is_closed())
    return "#" + get_short() + " is closed.";

  if(!can_receive(ob))
    return "#" + get_short() + " cannot hold " + get_short(ob) + ".";

  if(!can_hold_object(ob))
    return "#" + get_short() + " cannot hold " + get_short(ob) + ".";

  if(ob->is_container())
    return "#You cannot put a container inside another container.";

  return 1;
}

mixed direct_open_obj(object ob, string arg1) {
  if(!is_closeable())
    return 0;

  if(!is_closed())
    return "#" + get_short() + " is already open.";

  if(is_locked())
    return "#" + get_short() + " appears to be locked.";

  return 1;
}

mixed direct_close_obj(object ob, string arg1) {
  if(!is_closeable())
    return 0;

  if(is_closed())
    return "#" + get_short() + " is already closed.";

  return 1;
}

/**
 *
 * @param {STD_CONTAINER} container The container to be unlocked.
 * @param {OBJ_KEY} key The key to unlock the container.
 * @param {string} arg1 The container string supplied by the user.
 * @param {string} arg2 The key string supplied by the user.
 */
mixed direct_unlock_obj_with_obj(object container, object key, string arg1, string arg2) {
  if(!is_closeable())
    return 0;

  if(!container || !key)
    return 1;

  if(!container->is_container() || !key->is_key())
    return 0;

  if(container->query_key_id() != key->query_key_id())
    return "#That key does not turn this lock.";

  if(!is_closed())
    return "#" + get_short() + " is already open.";

  if(!is_locked())
    return "#" + get_short() + " does not appear to be locked.";

  return 1;
}

/**
 * Identifies this object as a container.
 *
 * Used for type checking.
 *
 * @returns {int} Always returns 1
 */
int is_container() { return 1; }
