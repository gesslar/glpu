/**
 * @file /std/room/door.c
 * @description Doors are properties that can be opened and closed.
 *
 * @created 2024-09-13 - Gesslar
 * @last_modified 2024-09-13 - Gesslar
 *
 * @history
 * 2024-09-13 - Gesslar - Created
 */

#include <classes.h>
#include <exits.h>
#include <door.h>

inherit CLASS_DOOR ;

private nosave mapping _doors = ([ ]) ;

int add_door(class Door door) {
  // Absolutely necessary in order to be able to add a door to a room.
  if(!stringp(door.direction))
    return null ;

  if(!valid_exit(door.direction))
    return null ;

  if(!nullp(_doors[door.direction]))
    return null ;

  // Fill in missing values.
  if(nullp(door.id))
    door.id = ({sprintf("%s door", door.direction)}) ;
  if(!pointerp(door.id))
    door.id = ({door.id}) ;
  door.id = distinct_array(door.id) ;

  if(nullp(door.open))
    door.open = 0 ;
  if(nullp(door.locked))
    door.locked = 0 ;

  _doors[door.direction] = door ;

  return 1 ;
}

int valid_door(string direction) {
  return !nullp(_doors[direction]) ;
}

int remove_door(string direction) {
  if(nullp(_doors[direction]))
    return null ;

  map_delete(_doors, direction) ;

  return 1 ;
}

int set_door_open(string direction, int bool) {
  if(nullp(_doors[direction]))
    return null ;

  bool = !!bool ;
  _doors[direction].open = bool ;

  return bool ;
}

int query_door_open(string direction) {
  if(nullp(_doors[direction]))
    return null ;

  return _doors[direction].open ;
}

int set_door_locked(string direction, int bool) {
  if(nullp(_doors[direction]))
    return null ;

  bool = !!bool ;
  _doors[direction].locked = bool ;

  return bool ;
}

int query_door_locked(string direction) {
  if(nullp(_doors[direction]))
    return null ;

  return _doors[direction].locked ;
}

int add_door_id(string direction, string id) {
  if(nullp(_doors[direction]))
    return null ;

  _doors[direction].id += ({id}) ;
  _doors[direction].id = distinct_array(_doors[direction].id) ;

  return 1 ;
}

int remove_door_id(string direction, string id) {
  if(nullp(_doors[direction]))
    return null ;

  _doors[direction].id = remove_array_element(_doors[direction].id, id) ;

  return 1 ;
}

string *query_door_ids(string direction) {
  if(nullp(_doors[direction]))
    return null ;

  return _doors[direction].id ;
}

int set_door_short(string direction, string short) {
  if(nullp(_doors[direction]))
    return null ;

  _doors[direction].short = short ;

  return 1 ;
}

string query_door_short(string direction) {
  if(nullp(_doors[direction]))
    return null ;

  return _doors[direction].short ;
}

int set_door_long(string direction, string long) {
  if(nullp(_doors[direction]))
    return null ;

  _doors[direction].long = long ;

  return 1 ;
}

string query_door_long(string direction) {
  if(nullp(_doors[direction]))
    return null ;

  return _doors[direction].long ;
}

mapping query_doors() {
  return copy(_doors) ;
}
