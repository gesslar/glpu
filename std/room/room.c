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


inherit STD_OBJECT ;
inherit STD_CONTAINER ;

inherit __DIR__ "exits" ;
inherit __DIR__ "light" ;
inherit __DIR__ "items" ;
inherit __DIR__ "zone" ;

void mudlib_setup(mixed args...) {
    set_ignore_capacity(1) ;
    set_ignore_mass(1) ;
}

int no_renew() { return 1 ; }
int is_room() { return 1; }
