/**
 * @file /std/room/room.c
 * @description A generic room object that can be inherited by any room.
 *
 * @created 2024-08-11 - Gesslar
 * @last_modified 2024-08-11 - Gesslar
 *
 * @history
 * 2024-08-11 - Gesslar - Created
 */

#include <room.h>

inherit STD_OBJECT;
inherit STD_CONTAINER;

inherit __DIR__ "exits";
inherit __DIR__ "items";
inherit __DIR__ "light";
inherit __DIR__ "terrain";
inherit __DIR__ "zone";
inherit __DIR__ "door";

private nosave int *_size = ({1, 1, 1});

void mudlib_setup(mixed args...) {
  set_ignore_capacity(1);
  set_ignore_mass(1);
  add_reset((: reset_doors :));
}

private nosave string room_type = "room";
private nosave string room_subtype = "";
private nosave string room_icon = "";
private nosave string room_environment = null;
private nosave int room_colour = null;
private nosave mapping custom_gmcp = ([ ]);
private nosave int no_gmcp_room_info = 0;
mapping gmcp_room_info(object who) {
  mapping info = ([ ]);
  string *exit_dirs;
  string *door_dirs;
  mapping exits;
  mapping doors;
  mapping result;
  mapping gmcp_info = ([ ]);
  // string *suppress;

  if(no_gmcp_room_info)
    return info;

  exit_dirs = query_exit_ids();
  door_dirs = query_door_ids();

  exits = query_exits();
  // suppress = query_exit_suppress() || ({});
  // exits = filter(exits, (: member_array($1, $(suppress)) == -1 :));
  exits = allocate_mapping(exit_dirs, (: query_exit :));
  exits = map(exits, (: base_name($2) :));
  exits = map(exits, (: hash("md4", $2) :));

  doors = query_doors();
  doors = map(doors, function(string dir, class Door door) {
    return ([
      "hash" : hash("md4", query_exit(dir)),
      "name" : door.name,
      "status" : query_door_status(dir, 1),
      "type" : door.type,
      "short" : door.short,
      "long" : door.long,
    ]);
  });

  result = ([
    "area"       : query_zone_name(),
    "hash"       : hash("md4", base_name()),
    "name"       : no_ansi(query_short()),
    "exits"      : exits,
    "doors"      : doors,
    "environment": query_room_environment() || query_terrain(),
    "coords"     : COORD_D->get_coordinates(base_name()),
    "size"       : _size,
    "type"       : room_type,
    "subtype"    : room_subtype,
    "icon"       : room_icon,
  ]);

  if(room_colour)
    result["color"] = COLOUR_D->colour_to_rgb(room_colour);

  gmcp_info = query_custom_gmcp();
  if(sizeof(gmcp_info))
    result["custom"] = gmcp_info;

  return result;
}

string set_room_environment(string environment) { room_environment = environment ; }
string query_room_environment() { return room_environment ; }

int set_room_colour(int colour) { room_colour = colour ; }
int query_room_colour() { return room_colour ; }

void set_room_size(int *size) {
  _size = size;
}

int *query_room_size() { return _size ; }

private nosave float _base_move_cost = 2.0;

float move_cost(string dir) {
  int *size = query_room_size();
  float cost;

  switch(dir) {
    case "north":
      cost = to_float(size[0]);
      break;
    case "south":
      cost = to_float(size[0]);
      break;
    case "east":
      cost = to_float(size[1]);
      break;
    case "west":
      cost = to_float(size[1]);
      break;
    case "northeast":
      cost = (to_float(size[0]) + to_float(size[1])) / 2.0;
      break;
    case "northwest":
      cost = (to_float(size[0]) + to_float(size[1])) / 2.0;
      break;
    case "southeast":
      cost = (to_float(size[0]) + to_float(size[1])) / 2.0;
      break;
    case "southwest":
      cost = (to_float(size[0]) + to_float(size[1])) / 2.0;
      break;
    case "up":
      cost = to_float(size[2]);
      break;
    case "down":
      cost = to_float(size[2]);
      break;
    default:
      cost = 1.0;
      break;
  }

  return cost * _base_move_cost;
}

int *get_virtual_coordinates() {
  string file = query_file_name();
  int x, y, z;

  if(sscanf(file, "%d,%d,%d", x, y, z) != 3)
    return null;

  return ({z, y, x});
}

void set_no_gmcp_room_info(int no_gmcp) {
  no_gmcp_room_info = no_gmcp;
}

void add_custom_gmcp(string key, mixed value) {
  custom_gmcp[key] = value;
}

void remove_custom_gmcp(string key) {
  map_delete(custom_gmcp, key);
}

mapping query_custom_gmcp() { return custom_gmcp ; }

string set_room_type(string type) { room_type = type ; }
string set_room_subtype(string subtype) { room_subtype = subtype ; }
string set_room_icon(string icon) { room_icon = icon ; }

string query_room_type() { return room_type ; }
string query_room_subtype() { return room_subtype ; }
string query_room_icon() { return room_icon ; }

int no_renew() { return 1 ; }
int is_room() { return 1 ; }
