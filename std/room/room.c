/*
    /std/room.c
    Tacitus@TLPUniversity
    Standard room inheritable
    * Special thanks to Icoz@LPUniversity
*/

// Last edited by Tacitus on May 3rd, 2006

inherit STD_OBJECT;
inherit STD_CONTAINER ;

inherit __DIR__ "exits" ;
inherit __DIR__ "light" ;
inherit __DIR__ "items" ;
inherit __DIR__ "zone" ;

int query_roomType();
string query_short();
string query_long();

void mudlib_setup(mixed args...) {
    set_ignore_capacity(1) ;
    set_ignore_mass(1) ;
}

int query_roomType() {
    return 1;
}

int no_renew() { return 1 ; }
int is_room() { return 1; }
