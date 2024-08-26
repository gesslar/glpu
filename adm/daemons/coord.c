/**
 * @file /adm/daemons/coord.c
 * @description Coordinate daemon to hold room data
 *
 * @created 2024-08-18 - Gesslar
 * @last_modified 2024-08-18 - Gesslar
 *
 * @history
 * 2024-08-18 - Gesslar - Created
 */

#include <classes.h>

inherit STD_DAEMON ;

inherit CLASS_ROOMINFO ;

private nomask mapping rooms = ([ ]) ;

void setup() {
    set_persistent(1) ;
}

void set_coordinate_data(mapping m) {
    rooms = m ;
    save_data() ;
}

mapping get_coordinate_data() {
    return rooms ;
}

int *get_coordinates(string room) {
    if(classp(rooms[room])) {
        return rooms[room].coords ;
    }

    return null ;
}

int *get_size(string room) {
    if(classp(rooms[room])) {
        return rooms[room].size ;
    }

    return null ;
}
