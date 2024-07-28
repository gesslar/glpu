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

private nosave string virtual_master = 0;

void create(mixed args...) {
    ::create(args...) ;

    if(!clonep()) {
        setup_chain(create_args...) ;
        reset() ;
    }
}

void virtual_start() {
    virtual_setup_chain(create_args...) ;
    reset() ;
}

int query_roomType() {
    return 1;
}

void set_virtual_master(string str) {
    virtual_master = str;
}

string query_virtual_master() {
    return virtual_master;
}

int query_ignore_mass() { return 1; }
int query_ignore_bulk() { return 1; }
int no_renew() { return 1 ; }
int is_room() { return 1; }
