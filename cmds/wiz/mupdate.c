/**
 * @file /cmds/wiz/mupdate.c
 * @description This command will cause the caller to be moved around
 *              all coordinates in order to generate a new Mudlet map.
 *
 * @created 2024-08-23 - Gesslar
 * @last_modified 2024-08-23 - Gesslar
 *
 * @history
 * 2024-08-23 - Gesslar - Created
 */

#include <classes.h>
#include <gmcp_defines.h>

inherit STD_CMD ;

inherit CLASS_ROOMINFO ;

void move_to_room(object tp, string *rooms, mapping data) ;

int done, sz ;

mixed main(object tp, string arg) {
    mapping rooms = copy(COORD_D->get_coordinate_data()) ;
    string *room_names = keys(rooms) ;

    sz = sizeof(room_names) ;
    done = 0 ;

    room_names = sort_array(room_names, 1) ;
    move_to_room(tp, room_names, rooms) ;

    return 1 ;
}

void move_to_room(object tp, string *rooms, mapping data, int it) {
    string room_file ;
    object room ;
    int *coords ;

    // room = rooms[0] ;
    room_file  = element_of(rooms) ;

    coords = data[room_file].coords ;

    done++ ;

    // tell(tp, sprintf("Sending GMCP Room Info for %s (x: %d, y: %d, z: %d)\n", file_name(load_object(room)), coords[0], coords[1], coords[2])) ;
    if(room = find_object(room_file))
        room->remove() ;

    GMCP_D->send_gmcp(tp, GMCP_PKG_ROOM_INFO, load_object(room_file)) ;

    rooms -= ({ room_file }) ;

    if(sizeof(rooms) > 0) {
        if(it > 250) {
            it = 0 ;
            // call_out_walltime((:move_to_room, tp, rooms, data, it:), 1) ;
            call_out_walltime((:move_to_room, tp, rooms, data, it:), 0.01) ;
            tell(tp, sprintf("Done %s/%s (%.2f%%)\n", add_commas(done), add_commas(sz - 1), percent(done*1.0, sz*1.0))) ;
        } else {
            call_out_walltime((:move_to_room, tp, rooms, data, ++it:), 0.01) ;
        }
    } else {
        tell(tp, sprintf("Done %s/%s (%.2f%%)\n", add_commas(done), add_commas(sz - 1), percent(done*1.0, sz*1.0))) ;
    }
}
