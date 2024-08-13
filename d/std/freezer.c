/**
 * @file /d/std/freezer.c
 * @description Room where bodies can hang out while not inhabited.
 *
 * @created 2024-08-11 - Gesslar
 * @last_modified 2024-08-11 - Gesslar
 *
 * @history
 * 2024-08-11 - Gesslar - Created
 */


#include <rooms.h>

inherit STD_ROOM;

void setup() {
     set_no_clean() ;
     set_light(1);
     set_short("The Freezer");
     set_long("Brrr!") ;
     set_exits(([
          "square" : ROOM_START,
     ]));
}
