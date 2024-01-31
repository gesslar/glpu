/*
    /std/room.c
    Tacitus@TLPUniversity
    Standard room inheritable
    * Special thanks to Icoz@LPUniversity
*/

// Last edited by Tacitus on May 3rd, 2006

#include <config.h>

inherit STD_OBJECT;
inherit __DIR__ "tell" ;
inherit __DIR__ "exits" ;
inherit __DIR__ "light" ;
inherit __DIR__ "items" ;

int query_roomType();
string query_short();
string query_long();

int query_roomType() {
    return 1;
}

string query_short() {
    if(::query_short()) return ::query_short();
    else return query("short");
}

string query_long() {
    if(::query_long()) return ::query_long();
    else return query("long");
}

int can_receive(object ob) {
    return 1;
}

void receive_effect(object ob) {}

int can_release(object ob) {
    return 1;
}

void release_effect(object ob) {}
