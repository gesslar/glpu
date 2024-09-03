// /std/area/rooms/zone.c
// Room zone module
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

string find_path(string path) ;

private nosave object zone ;

void set_zone(mixed z) {
    if(stringp(z)) {
        z = find_path(z) ;
        z = load_object(z) ;
    }

    if(!objectp(z)) {
        error("Invalid zone object: " + z) ;
    }

    if(!z->is_zone()) {
        error("Invalid zone object: " + z) ;
    }

    zone = z ;
    zone->add_room(this_object()) ;
}

string query_zone_name() {
    return zone->query_zone_name() ;
}

object query_zone() {
    return zone ;
}
