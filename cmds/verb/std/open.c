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
  add_rules(({"", "STR", "OBJ"}), ({}));
}

mixed can_open() { return "Open what?"; }

mixed can_open_str(mixed direction) {
  /** @type {STD_PLAYER} */
  object user = this_body();
  /** @type {STD_ROOM} */
  object room = environment(user);
  mixed result;

  direction = resolve_direction(direction);
  result = room->can_open_door(direction);

  return result;
}

mixed do_open_str(mixed direction) {
  /** @type {STD_PLAYER} */
  object user = this_body();
  /** @type {STD_ROOM} */
  object room = environment(user);
  /** @type {STD_ROOM} */
  object other_room;
  mixed result;
  string *directions;

  direction = resolve_direction(direction);

  if((result = room->can_open_door(direction)) == 1) {
    room->set_door_open(direction, true, true);

    user->simple_action("$N $vopen the $o.\n", room->query_door_name(direction));

    other_room = room->query_exit_dest(direction);
    if(other_room) {
      foreach(string dir in other_room->query_exit_ids()) {
        if(other_room->query_exit_dest(dir) == room) {
            other_room->set_door_open(dir, true);
            break;
        }
      }
    }

    return 1;
  }

  return result;
}

private string resolve_direction(string direction) {
  /** @type {STD_PLAYER} */
  object user = this_body();
  /** @type {STD_ROOM} */
  object room = environment(user);
  /** @type {STD_ROOM} */
  string *directions;

  directions = room->id_door(direction);

  if(!sizeof(directions))
    return 0;

  return directions[0];
}
