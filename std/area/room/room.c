/*
    /std/room.c
    Tacitus@TLPUniversity
    Standard room inheritable
    * Special thanks to Icoz@LPUniversity
*/

// Last edited by Tacitus on May 3rd, 2006

#include <config.h>

inherit STD_OBJECT;
inherit __DIR__ "exits" ;
inherit __DIR__ "light" ;
inherit __DIR__ "items" ;
inherit __DIR__ "zone" ;

int query_roomType();
string query_short();
string query_long();

private nosave string virtual_master = 0;

int query_roomType() {
    return 1;
}

int can_receive(object ob) {
    return 1;
}
void receive_effect(object ob) {}

int can_release(object ob) {
    return 1;
}
void release_effect(object ob) {}

void set_virtual_master(string str) {
    virtual_master = str;
}

string query_virtual_master() {
    return virtual_master;
}

int query_ignore_mass() { return 1; }
int query_ignore_bulk() { return 1; }
int is_room() { return 1; }
