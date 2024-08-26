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

private nosave int *_size = ({1, 1, 1}) ;

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
        "hash"       : hash("md4", base_name()),
        "name"       : no_ansi(query_short()),
        "exits"      : exits,
        "environment": query_terrain(),
        "coords"     : COORD_D->get_coordinates(base_name()),
        "size"       : _size,
        "type"       : room_type,
        "subtype"    : room_subtype,
        "icon"       : room_icon,
    ]) ;
}

void set_room_size(int *size) {
    _size = size ;
}

int *query_room_size() { return _size ; }

private nosave float _base_move_cost = 2.0 ;

float move_cost(string dir) {
    int *size = query_room_size() ;
    float cost ;

    switch(dir) {
        case "north":
            cost = to_float(size[0]) ;
            break ;
        case "south":
            cost = to_float(size[0]) ;
            break ;
        case "east":
            cost = to_float(size[1]) ;
            break ;
        case "west":
            cost = to_float(size[1]) ;
            break ;
        case "northeast":
            cost = (to_float(size[0]) + to_float(size[1])) / 2.0 ;
            break ;
        case "northwest":
            cost = (to_float(size[0]) + to_float(size[1])) / 2.0 ;
            break ;
        case "southeast":
            cost = (to_float(size[0]) + to_float(size[1])) / 2.0 ;
            break ;
        case "southwest":
            cost = (to_float(size[0]) + to_float(size[1])) / 2.0 ;
            break ;
        case "up":
            cost = to_float(size[2]) ;
            break ;
        case "down":
            cost = to_float(size[2]) ;
            break ;
        default:
            cost = 1.0 ;
            break ;
    }

    return cost * _base_move_cost ;
}

string set_room_type(string type) { room_type = type ; }
string set_room_subtype(string subtype) { room_subtype = subtype ; }
string set_room_icon(string icon) { room_icon = icon ; }

string query_room_type() { return room_type ; }
string query_room_subtype() { return room_subtype ; }
string query_room_icon() { return room_icon ; }

int no_renew() { return 1 ; }
int is_room() { return 1; }
