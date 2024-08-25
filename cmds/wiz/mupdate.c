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

mixed main(object tp, string arg) {
    mapping rooms = copy(COORD_D->get_coordinate_data()) ;
    string *room_names = keys(rooms) ;

    // room_names = sort_array(room_names, 1) ;
    move_to_room(tp, room_names, rooms) ;

    return 1 ;
}

void move_to_room(object tp, string *rooms, mapping data) {
    string room ;
    int *coords ;

    room = rooms[0] ;
    // room = element_of(rooms) ;

    coords = data[room].coords ;

    tell(tp, sprintf("Sending GMCP Room Info for %s (x: %d, y: %d, z: %d)\n", file_name(load_object(room)), coords[0], coords[1], coords[2])) ;
    GMCP_D->send_gmcp(tp, GMCP_PKG_ROOM_INFO, load_object(room)) ;

    rooms -= ({ room }) ;

    if(sizeof(rooms) > 0)
        call_out_walltime((:move_to_room, tp, rooms, data:), 0.01) ;
}
