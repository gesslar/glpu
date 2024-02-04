// /std/area/rooms/zone.c
// Room zone module
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

private nosave object zone ;

void set_zone(mixed z) {
    if(stringp(z)) {
        z = append(z, ".c") ;
        if(file_size(z) < 1) {
            error("Zone file does not exist: " + z) ;
        }
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
    debug_message(sprintf("Zone set to: %O\n", z)) ;
}

object query_zone() {
    return zone ;
}
