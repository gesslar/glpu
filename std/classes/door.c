/**
 * @file /std/classes/door.c
 *
 * Class definition for Door objects used in room implementations.
 *
 * @created 2024-09-13 - Gesslar
 * @last_modified 2025-03-16 - GitHub Copilot
 *
 * @history
 * 2024-09-13 - Gesslar - Created
 * 2025-03-16 - GitHub Copilot - Added documentation
 */

#ifndef __DOOR_C__
#define __DOOR_C__

/**
 * Represents a door in a room that can be opened, closed, and locked.
 *
 * @property {string*} id - Array of identifiers for the door
 * @property {string} name - The display name of the door
 * @property {string} status - The current state of the door (open/closed/locked)
 * @property {string} type - The type of door (door, gate, etc.)
 * @property {string} short - Short description of the door
 * @property {string} long - Detailed description of the door
 * @property {string} direction - The direction/exit where the door is located
 */
class Door {
  string *id;
  string name;
  string status;
  string type;
  string short;
  string long;
  string direction;
}

#endif // __DOOR_C__
