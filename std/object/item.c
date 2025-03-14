/**
 * @file /std/object/item.c
 *
 * Actual objects, whereas object is the base of everything
 *
 * @created 2024-07-27 - Gesslar
 * @last_modified 2024-07-27 - Gesslar
 *
 * @history
 * 2024-07-27 - Gesslar - Created
 */

#include <gmcp_defines.h>

inherit STD_OBJECT;
inherit STD_VALUE;

private nosave mapping spawn_info = ([]);

void set_spawn_info(mapping info) {
  spawn_info = info;
}

void add_spawn_info(string key, mixed value) {
  spawn_info[key] = value;
}

mixed query_spawn_info(string key) {
  return spawn_info[key];
}

mapping query_all_spawn_info() {
  return copy(spawn_info);
}

int allow_move(mixed dest) {
  /** @type {STD_CONTAINER} */
  object ob;
  /** @type {STD_CONTAINER} */
  object env;

  if(stringp(dest))
    catch(ob = load_object(dest));
  else if(objectp(dest))
    ob = dest;

  if(!objectp(ob))
    return MOVE_NO_DEST;

  if(environment() == ob)
    return MOVE_ALREADY_THERE;

  if(!ob->can_receive(this_object()))
    return MOVE_NOT_ALLOWED;

  if(mud_config("USE_MASS"))
    if(!dest->ignore_capacity())
      if(query_mass() + dest->query_fill() > dest->query_capacity())
        return MOVE_TOO_HEAVY;

  if(env)
    if(!env->can_release(this_object()))
      return 0;

  return MOVE_OK;
}

private void roll_back(mixed *rollback) {
  int i = sizeof(rollback);

  while(i--) {
    if(objectp(rollback[i][0]))
      rollback[i][0]->adjust_mass(rollback[i][1][0]);
    if(objectp(rollback[i][0]))
      rollback[i][0]->adjust_fill(rollback[i][1][1]);
  }
}

/**
 *
 * @param {string|object STD_CONTAINER} dest - The destination to move to.
 */
int move(mixed dest) {
  int result;
  /** @type {STD_CONTAINER} */
  object prev = environment();
  int use_mass = mud_config("USE_MASS");
  int prev_ignore_capacity = prev ? call_if(prev, "ignore_capacity") : 0;
  int dest_ignore_capacity = call_if(dest, "ignore_capacity") || 0;
  int prev_ignore_mass = prev ? call_if(prev, "ignore_mass") : 0;
  int dest_ignore_mass = call_if(dest, "ignore_mass") || 0;
  int mass = query_mass();
  string e;

  mixed *rollback = ({});

  result = allow_move(dest);

  if(result)
    return result;

  if(!objectp(dest)) {
    if(stringp(dest)) {
      e = catch(dest = load_object(dest));

      if(stringp(e) || !objectp(dest))
        return MOVE_NO_DEST;
    }
    else
      return MOVE_NO_DEST;
  }

  if(prev && prev == dest)
    return MOVE_ALREADY_THERE;

  // 0 = prev, 1 = dest
  rollback = allocate(2);

  if(use_mass) {
    // 0 = object, 1 = ({ mass, fill})
    rollback[0] = allocate(2);

    // Update the mass and fill of the previous environment.
    rollback[0][0] = prev;
    rollback[0][1] = allocate(2, 0);
    if(prev) {
      if(!prev_ignore_mass) {
        if(prev->adjust_mass(-mass))
          rollback[0][1][0] = mass;
        else
          return MOVE_TOO_HEAVY;
      }

      if(!prev_ignore_capacity) {
        if(prev->adjust_fill(-mass))
          rollback[0][1][1] = mass;
        else {
          roll_back(rollback);
          return MOVE_TOO_HEAVY;
        }
      }
    }

    // Update the mass and fill of the new environment. Rolling back if we fail,
    // both the new and the previous environment.
    rollback[1] = allocate(2);
    rollback[1][0] = dest;
    rollback[1][1] = allocate(2, 0);
    if(!dest_ignore_mass) {
      if(dest->adjust_mass(mass))
        rollback[1][1][0] = -mass;
      else {
        roll_back(rollback);
        return MOVE_TOO_HEAVY;
      }
    }

    if(!dest_ignore_capacity) {
      if(dest->adjust_fill(mass))
        rollback[1][1][1] = -mass;
      else {
        roll_back(rollback);
        return MOVE_TOO_HEAVY;
      }
    }
  }

  flush_messages();

  // Ok, we can move now.
  move_object(dest);

  event(this_object(), "moved", prev);
  if(prev && this_object()) {
    event(prev, "released", environment());
    event(prev, "gmcp_item_remove", prev);
  }

  if(this_object()) {
    event(environment(), "received", prev);
    event(environment(), "gmcp_item_add", environment());
  }

  if(userp())
    GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_ITEMS_LIST, GMCP_LIST_ROOM);

  if(this_object())
    return MOVE_OK;
  else {
    roll_back(rollback);
    return MOVE_DESTRUCTED;
  }
}
