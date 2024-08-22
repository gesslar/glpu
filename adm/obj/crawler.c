/**
 * @file /adm/obj/crawler.c
 * @description This object crawls the mud to get room exit information.
 *
 * @created 2024-08-18 - Gesslar
 * @last_modified 2024-08-18 - Gesslar
 *
 * @history
 * 2024-08-18 - Gesslar - Created
 */


#include <classes.h>

inherit STD_ITEM ;

inherit CLASS_ROOMINFO ;

void crawl_next_room() ;
void process_room(object room) ;
int *update_coordinates(int *coords, string exit, int *current_size, int *next_size) ;

private nosave float crawl_speed = 0.15 ;
private mapping done = ([]) ;
private mapping todo = ([]) ;
private int x, y, z ;

void setup() {
    set_name("crawler") ;
    set_short("crawler") ;
    set_long("This is a crawler.") ;
    add_extra_long("crawl", "To start crawling, type 'crawl'.") ;

    add_command("crawl", "cmd_crawl") ;
}

mixed cmd_crawl(object tp, string arg) {
    object room = environment(tp) ;
    if(!room) {
        return "You need to be in a room to start crawling.\n" ;
    }

    todo[file_name(room)] = new(class RoomInfo,
        short: room->query_short(),
        todo: room->query_exit_ids(),
        done: ({}),
        coords: ({0, 0, 0}),
        size: ({1, 1, 1})  // Default size to 1x1x1
    ) ;
    call_out_walltime("crawl_next_room", crawl_speed) ;

    return "You begin crawling from " + room->query_short() + ".\n" ;
}

void crawl_next_room() {
    string file ;
    object room ;

    if(!sizeof(todo)) {
        tell_room(environment(this_object()), "Crawling complete. All rooms processed.\n") ;
        tell_room(environment(this_object()), sprintf("Total rooms discovered: %d\n", sizeof(done))) ;
        COORD_D->set_coordinate_data(done) ;
        done = ([ ]) ;
        todo = ([ ]) ;
        return ;
    }

    file = keys(todo)[0] ;
    room = load_object(file) ;

    if(!room) {
        map_delete(todo, file) ;
        call_out_walltime("crawl_next_room", crawl_speed) ;
        return ;
    }

    process_room(room) ;
}

void process_room(object room) {
    string file = file_name(room) ;
    class RoomInfo room_data = todo[file] ;
    string exit ;
    string dest ;

    if(!room_data->done) {
        tell_room(environment(this_object()), sprintf("Processing %s at (%d,%d,%d), size (%d,%d,%d).\n",
            room_data->short, room_data->coords[0], room_data->coords[1], room_data->coords[2],
            room_data->size[0], room_data->size[1], room_data->size[2])) ;
    }

    while(sizeof(room_data->todo)) {
        exit = room_data->todo[0] ;
        dest = room->query_exit(exit) ;

        if(dest && !done[dest] && !todo[dest]) {
            object next_room = load_object(dest) ;
            if(next_room) {
                int *next_size = next_room->query_room_size() || ({1, 1, 1}) ;  // Default to 1x1x1 if not set
                int *new_coords = update_coordinates(room_data->coords, exit, room_data->size, next_size) ;
                todo[dest] = new(class RoomInfo,
                    short: next_room->query_short(),
                    todo: next_room->query_exit_ids(),
                    done: ({}),
                    coords: new_coords,
                    size: next_size
                ) ;
                tell_room(environment(this_object()), sprintf("Discovered %s at (%d,%d,%d), size (%d,%d,%d) via %s.\n",
                    todo[dest]->short, new_coords[0], new_coords[1], new_coords[2],
                    next_size[0], next_size[1], next_size[2], exit)) ;
            }
        }

        room_data->todo = room_data->todo[1..] ;
        room_data->done += ({ exit }) ;
    }

    done[file] = room_data ;
    map_delete(todo, file) ;
    call_out_walltime("crawl_next_room", crawl_speed) ;
}

int *update_coordinates(int *coords, string exit, int *current_size, int *next_size) {
    float *new_coords = ({ to_float(coords[0]), to_float(coords[1]), to_float(coords[2]) }) ;
    switch(exit) {
        case "north": new_coords[1] += (current_size[1] + next_size[1]) / 2.0 ; break ;
        case "south": new_coords[1] -= (current_size[1] + next_size[1]) / 2.0 ; break ;
        case "east":  new_coords[0] += (current_size[0] + next_size[0]) / 2.0 ; break ;
        case "west":  new_coords[0] -= (current_size[0] + next_size[0]) / 2.0 ; break ;
        case "up":    new_coords[2] += (current_size[2] + next_size[2]) / 2.0 ; break ;
        case "down":  new_coords[2] -= (current_size[2] + next_size[2]) / 2.0 ; break ;
        case "northeast":
            new_coords[0] += (current_size[0] + next_size[0]) / 2.0 ;
            new_coords[1] += (current_size[1] + next_size[1]) / 2.0 ;
            break ;
        case "northwest":
            new_coords[0] -= (current_size[0] + next_size[0]) / 2.0 ;
            new_coords[1] += (current_size[1] + next_size[1]) / 2.0 ;
            break ;
        case "southeast":
            new_coords[0] += (current_size[0] + next_size[0]) / 2.0 ;
            new_coords[1] -= (current_size[1] + next_size[1]) / 2.0 ;
            break ;
        case "southwest":
            new_coords[0] -= (current_size[0] + next_size[0]) / 2.0 ;
            new_coords[1] -= (current_size[1] + next_size[1]) / 2.0 ;
            break ;
    }
    return ({ to_int(round(new_coords[0])), to_int(round(new_coords[1])), to_int(round(new_coords[2])) }) ;
}
