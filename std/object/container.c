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
int closeable() {
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
int lockable() {
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

/**
 * Identifies this object as a container.
 *
 * Used for type checking.
 *
 * @returns {int} Always returns 1
 */
int is_container() { return 1 ; }
