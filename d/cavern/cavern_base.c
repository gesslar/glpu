/**
 * @file /d/cavern/cavern_base.c
 * @description The base room for the caverns
 *
 * @created 2024-09-18 - Gesslar
 * @last_modified 2024-09-18 - Gesslar
 *
 * @history
 * 2024-09-18 - Gesslar - Created
 */

inherit STD_ROOM ;

void setup() {
  set_light(0) ;
  set_terrain("road") ;
}

void virtual_setup(mixed args...) {
  string file = args[0] ;

  set_zone("winding_caverns") ;
  __DIR__ "cavern_daemon"->setup_room_type(this_object()) ;
  __DIR__ "cavern_daemon"->setup_room_subtype(this_object()) ;
  __DIR__ "cavern_daemon"->setup_exits(this_object(), file) ;
  __DIR__ "cavern_daemon"->setup_short(this_object(), file) ;
  __DIR__ "cavern_daemon"->setup_long(this_object(), file) ;
  // _debug("Room type for %O is %O", this_object(), query_room_environment()) ;
}
