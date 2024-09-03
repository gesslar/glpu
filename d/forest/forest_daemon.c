/**
 * @file /d/village/forest/forest_daemon.c
 * @description Daemon for managing the forest area
 *
 * @created 2024-08-23 - Gesslar
 * @last_modified 2024-08-23 - Gesslar
 *
 * @history
 * 2024-08-23 - Gesslar - Created
 */

inherit STD_VIRTUAL_MAP ;

// Forward declarations
private void setup_forest_shorts() ;
private void setup_forest_longs() ;

private nosave string *forest_shorts ;
private nosave string *forest_longs ;
private nosave string clearing_long ;
private nosave string forest_map_file ;

private nosave int rot = 0;

void setup() {
    set_map_file(__DIR__ "forest_map.txt") ;
    setup_forest_shorts() ;
    setup_forest_longs() ;
}

private void setup_forest_shorts() {
    forest_shorts = ({
        "Shadowy Forest",
        "Dense Woodland",
        "Lush Woodland",
        "Thick Forest",
    });
}

private void setup_forest_longs() {
    forest_longs = ({
        "You are surrounded by tall trees, their branches intertwining above "
        "you to form a thick canopy. The forest floor is covered with fallen "
        "leaves and small plants. The air is cool and damp, filled with the "
        "scents of earth and vegetation.",

        "The forest here is dense, with trees growing close together. Shafts "
        "of sunlight filter through the leaves, creating a dappled pattern on "
        "the ground. The air is filled with the sounds of birds and small "
        "animals moving through the underbrush.",

        "A hushed silence permeates this part of the forest. The trees stand "
        "close together, their leaves rustling softly in the breeze. The air "
        "is thick with the scent of pine and damp earth, giving the area a "
        "primeval feel.",

        "The forest is particularly thick here, with ancient trees towering "
        "overhead. Their massive trunks are covered in moss and lichen, and "
        "their roots create a maze-like pattern on the forest floor. The "
        "canopy above is so dense that little direct sunlight reaches the "
        "ground.",
    }) ;

    clearing_long =
        "You've stumbled upon a small clearing in the dense forest. Sunlight "
        "streams down, illuminating a patch of wildflowers and soft grass. "
        "The surrounding trees seem to form a natural barrier, creating a "
        "peaceful sanctuary within the woods." ;
}

public void setup_short(object room, string file) {
    int x, y, z;
    string room_type;

    sscanf(file, "%d,%d,%d", x, y, z);
    room_type = get_room_type(x, y);

    if(room_type == "O")
        room->set_short(element_of(forest_shorts));
    else if(room_type == "X")
        room->set_short("Forest Clearing");
}

public void setup_long(object room, string file) {
    int x, y, z;
    string room_type;

    sscanf(file, "%d,%d,%d", x, y, z);
    room_type = get_room_type(x, y);

    if(room_type == "O") {
        room->set_long(forest_longs[rot]);
        if(++rot >= sizeof(forest_longs))
            rot = 0;
    } else if(room_type == "X")
        room->set_long(clearing_long);
}

public void setup_exits(object room, string file) {
    int x, y, z;
    string room_type;
    mapping exits;

    if(sscanf(file, "%d,%d,%d", x, y, z) != 3)
        return;

    room_type = get_room_type(x, y);
    exits = get_exits(x, y, z);

    if(file == "2,0,0")
        exits["north"] = "../village/village_path5";
    if(file == "27,11,0")
        exits["east"] = "../wastes/0,0,0" ;

    room->set_exits(exits);
}
