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
inherit __DIR__ "items" ;
inherit __DIR__ "light" ;
inherit __DIR__ "terrain" ;
inherit __DIR__ "zone" ;

void mudlib_setup(mixed args...) {
    set_ignore_capacity(1) ;
    set_ignore_mass(1) ;
}

private nosave string room_type = "room" ;
private nosave string room_subtype = "" ;
private nosave string room_icon = "" ;

mapping gmcp_room_info(object who) {
    mapping info = ([ ]) ;
    string *exit_dirs = query_exit_ids() ;
    mapping exits ;
    // string *suppress ;

    exits = query_exits() ;
    // suppress = query_exit_suppress() || ({}) ;
    // exits = filter(exits, (: member_array($1, $(suppress)) == -1 :));
    exits = allocate_mapping(exit_dirs, (: query_exit :)) ;
    exits = map(exits, (: base_name($2) :));
    exits = map(exits, (: hash("md4", $2) :));

    return ([
        "area"       : "Olum",
        "roomhash"   : hash("md4", base_name()),
        "name"       : no_ansi(query_short()),
        "exits"      : exits,
        "environment": query_terrain(),
        "coords"     : COORD_D->get_coordinates(base_name()),
        "type"       : room_type,
        "subtype"    : room_subtype,
        "icon"       : room_icon,
    ]) ;
}

string set_room_type(string type) { room_type = type ; }
string set_room_subtype(string subtype) { room_subtype = subtype ; }
string set_room_icon(string icon) { room_icon = icon ; }

string query_room_type() { return room_type ; }
string query_room_subtype() { return room_subtype ; }
string query_room_icon() { return room_icon ; }

int no_renew() { return 1 ; }
int is_room() { return 1; }
