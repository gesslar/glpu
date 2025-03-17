/**
 * @file /std/room/door.c
 * @description Doors are properties that can be opened and closed.
 *
 * @created 2024-09-13 - Gesslar
 * @last_modified 2025-03-16 - GitHub Copilot
 *
 * @history
 * 2024-09-13 - Gesslar - Created
 * 2025-03-16 - GitHub Copilot - Added documentation
 */

#include <classes.h>
#include <exits.h>
#include <door.h>
#include <gmcp_defines.h>

inherit CLASS_DOOR;

private nosave mapping _doors = ([ ]);

/**
 * Adds a door to a room in the specified direction.
 *
 * This will automatically populate missing values with sensible defaults
 * and ensure door IDs are unique.
 *
 * @param {class Door} door - The door object to add
 * @returns {int} 1 on success, 0 on failure
 */
int add_door(class Door door) {
  // Absolutely necessary in order to be able to add a door to a room.
  if(!stringp(door.direction))
    return null;

  if(!valid_exit(door.direction))
    return null;

  if(!nullp(_doors[door.direction]))
    return null;

  // Fill in missing values.
  if(nullp(door.id))
    door.id = ({sprintf("%s door", door.direction)});
  if(!pointerp(door.id))
    door.id = ({door.id});
  door.id = distinct_array(door.id);

  if(!door.status)
    door.status = "closed";

  if(!door.type)
    door.type = "door";

  if(!door.name)
    door.name = sprintf("%s %s", door.direction, door.type);

  door.id += ({ door.name, door.type, lower_case(door.short) });

  _doors[door.direction] = door;

  GMCP_D->broadcast_gmcp(this_object(), GMCP_PKG_ROOM_INFO, this_object());

  return 1;
}

/**
 * Checks if a valid door exists in the specified direction.
 *
 * @param {string} direction - The direction to check
 * @returns {int} 1 if door exists, null if not
 */
int valid_door(string direction) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  return 1;
}

/**
 * Removes a door from the specified direction.
 *
 * @param {string} direction - The direction of the door to remove
 * @returns {int} 1 on success, null if door doesn't exist
 */
int remove_door(string direction) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  map_delete(_doors, direction);

  GMCP_D->broadcast_gmcp(this_object(), GMCP_PKG_ROOM_INFO, this_object());

  return 1;
}

/**
 * Sets a door's open status.
 *
 * @param {string} direction - The direction of the door
 * @param {int} bool - 1 to open door, 0 to close door
 * @param {int} [silent=0] - If 1, suppresses messages about state changes
 * @returns {int} The new state (1=open, 0=closed), or null on failure
 */
varargs int set_door_open(string direction, int bool, int silent) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  if(nullp(bool))
    return null;

  bool = !!bool;
  door.status = bool ? "open" : "closed";

  _doors[direction] = door;

  if(!silent)
    tell_down(this_object(),
      sprintf("The %s is now %s.\n", door.name, door.status));

  GMCP_D->broadcast_gmcp(this_object(), GMCP_PKG_ROOM_INFO, this_object());
  return bool;
}

/**
 * Checks if a door in the specified direction is open.
 *
 * @param {string} direction - The direction to check
 * @returns {int} 1 if door is open, 0 if closed or locked, null if door doesn't exist
 */
int query_door_open(string direction) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  return door.status == "open";
}

/**
 * Sets a door's locked status.
 *
 * Cannot lock an open door.
 *
 * @param {string} direction - The direction of the door
 * @param {int} bool - 1 to lock door, 0 to unlock door
 * @param {int} [silent=0] - If 1, suppresses messages about state changes
 * @returns {int} The new lock state (1=locked, 0=unlocked), or null on failure
 */
int set_door_locked(string direction, int bool, int silent) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  if(nullp(bool))
    return null;

  if(door.status == "open")
    return 0;

  if(nullp(silent))
    silent = 0;

  silent = !!silent;
  bool = !!bool;

  door.status = bool ? "locked" : "closed";

  if(!silent)
    tell_down(this_object(),
      sprintf("There is a click from the %s.\n", door.name));

  _doors[direction] = door;

  GMCP_D->broadcast_gmcp(this_object(), GMCP_PKG_ROOM_INFO, this_object());

  return bool;
}

/**
 * Checks if a door in the specified direction is locked.
 *
 * @param {string} direction - The direction to check
 * @returns {int} 1 if door is locked, 0 if not locked, null if door doesn't exist
 */
int query_door_locked(string direction) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  return door.status == "locked";
}

/**
 * Adds an ID to a door in the specified direction.
 *
 * @param {string} direction - The direction of the door
 * @param {string} id - The ID to add
 * @returns {int} 1 on success, null if door doesn't exist
 */
int add_door_id(string direction, string id) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  door.id += ({id});
  door.id = distinct_array(door.id);

  _doors[direction] = door;

  return 1;
}

/**
 * Removes an ID from a door in the specified direction.
 *
 * @param {string} direction - The direction of the door
 * @param {string} id - The ID to remove
 * @returns {int} 1 on success, null if door doesn't exist
 */
int remove_door_id(string direction, string id) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  door.id = remove_array_element(door.id, id);

  _doors[direction] = door;

  return 1;
}

/**
 * Returns all directions that have doors.
 *
 * @returns {string*} Array of direction strings
 */
string *query_door_ids() {
  return keys(_doors);
}

/**
 * Sets a door's short description.
 *
 * @param {string} direction - The direction of the door
 * @param {string} short - The short description
 * @returns {int} 1 on success, null if door doesn't exist
 */
int set_door_short(string direction, string short) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  door.short = short;

  _doors[direction] = door;

  return 1;
}

/**
 * Returns a door's short description.
 *
 * @param {string} direction - The direction of the door
 * @returns {string} The door's short description, or null if door doesn't exist
 */
string query_door_short(string direction) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  return door.short;
}

/**
 * Sets a door's long description.
 *
 * @param {string} direction - The direction of the door
 * @param {string} long - The long description
 * @returns {int} 1 on success, null if door doesn't exist
 */
int set_door_long(string direction, string long) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  door.long = long;

  _doors[direction] = door;

  return 1;
}

/**
 * Returns a door's long description.
 *
 * @param {string} direction - The direction of the door
 * @returns {string} The door's long description, or null if door doesn't exist
 */
string query_door_long(string direction) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  return door.long;
}

/**
 * Returns a copy of all doors in the room.
 *
 * @returns {mapping} Mapping of direction to door object
 */
mapping query_doors() {
  return copy(_doors);
}

/**
 * Returns a door's name.
 *
 * @param {string} direction - The direction of the door
 * @returns {string} The door's name, or null if door doesn't exist
 */
string query_door_name(string direction) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  return door.name;
}

/**
 * Returns all directions of doors that match the given ID.
 *
 * @param {string} id - The ID to match
 * @returns {string*} Array of directions with doors matching the ID
 */
string *id_door(string id) {
  string *doors = ({ });

  foreach(string dir, class Door door in _doors)
    if(of(id, door.id))
      doors += ({ dir });

  return doors;
}

/**
 * Synchronizes door states with the doors in connected rooms.
 *
 * This ensures doors have the same state on both sides of an exit.
 * Removes doors if their exit no longer exists.
 */
void reset_doors() {
  string *exits = query_exit_ids();

  foreach(string dir, class Door door in _doors) {
    string other_room_file;

    // Sanity check.
    if(member_array(dir, exits) == -1) {
      remove_door(dir);
      continue;
    }

    if(other_room_file = query_exit(dir)) {
      object other_room;

      other_room = find_object(other_room_file);

      if(!objectp(other_room))
        continue;

      foreach(string other_dir in other_room->query_exit_ids()) {
        if(other_room->query_exit_dest(other_dir, true) == this_object()) {
          if(other_room->query_door_open(other_dir)) {
            set_door_open(dir, true, true);
            break;
          } else if(other_room->query_door_locked(other_dir)) {
            set_door_locked(dir, true, true);
            break;
          }
        }
      }
    }
  }
}

/**
 * Returns a door's status.
 *
 * @param {string} direction - The direction of the door
 * @param {int} [as_number=0] - If true, returns status as a number (1=open, 2=closed, 3=locked)
 * @returns {mixed} The door's status as string or number, or null if door doesn't exist
 */
varargs mixed query_door_status(string direction, int as_number) {
  class Door door;

  if(nullp(door = _doors[direction]))
    return null;

  if(as_number) {
    switch(door.status) {
      case "open": return 1;
      case "closed": return 2;
      case "locked": return 3;
      default: return 0;
    }
  }

  return door.status;
}
