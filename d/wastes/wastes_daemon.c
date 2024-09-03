/**
 * @file /d/wastes/wastes_daemon.c
 * @description Virtual map daemon for the wastes
 *
 * @created 2024-08-30 - Gesslar
 * @last_modified 2024-08-30 - Gesslar
 *
 * @history
 * 2024-08-30 - Gesslar - Created
 */

inherit STD_VIRTUAL_MAP;

inherit M_NOISE;

private void setup_wastes_shorts() ;
private void setup_wastes_longs() ;
private void generate_map() ;
string determine_room_type(int x, int y) ;

private nosave string *wastes_shorts ;
private nosave string *wastes_longs ;
private nosave string wastes_long ;
private nosave int rot = 0 ;
private nosave int *dimensions = ({25, 25}) ;

void setup() {
    // init_noise(dimensions[0], dimensions[1], BOOT_D->query_boot_number()) ;
    init_noise(dimensions[0], dimensions[1], 41) ;
    set_noise_map((: generate_map :)) ;
    setup_wastes_shorts();
    setup_wastes_longs();
}

protected mixed *generate_map() {
    mixed map;
    mapping map_stuff = ([]);

    map = allocate(dimensions[0]);
    for(int i = 0; i < dimensions[0]; i++)
        map[i] = allocate(dimensions[1]);

    for (int x = 0; x < dimensions[0]; x++) {
        map[x] = allocate(dimensions[1]);
        for (int y = 0; y < dimensions[1]; y++) {
            float noise_value =
                simplex2(to_float(x) /  5, to_float(y) /  5) +
                simplex2(to_float(x) / 10, to_float(y) / 10) +
                simplex2(to_float(x) / 15, to_float(y) / 15) ;
            int result ;

            noise_value = noise_value / 3.0;

            result = to_int(((noise_value + 1.0) * 5.0)) ;
            update_min_max(result); // Update min/max values
            map[x][y] = result; // Store the noise value in the map
            map_stuff[result]++;
        }
    }

    // _debug("Map Stuff: %O", map_stuff);
    // _debug("Min: %O, Max: %O", get_noise_map_range()[0], get_noise_map_range()[1]);
    return map;
}

private void setup_wastes_shorts() {
    wastes_shorts = ({
        "Barren Expanse",
        "Scorched Earth",
        "Desolate Stretch",
        "Rugged Terrain",
    });
}

private void setup_wastes_longs() {
    wastes_longs = ({
        "The wasteland stretches before you, a desolate expanse of cracked earth "
        "and scattered debris. The air is dry and carries the faint scent of decay, "
        "with the sun beating down mercilessly.",

        "This area of the wasteland is littered with remnants of a forgotten past, "
        "rusted metal and broken glass glinting in the harsh light. The ground is "
        "uneven, with deep fissures and patches of thorny vegetation.",

        "A chilling wind sweeps across the wasteland, stirring up dust and debris. "
        "The landscape is dotted with skeletal remains of long-dead trees, their "
        "twisted branches reaching out like bony fingers.",

        "Silence reigns in this part of the wasteland, broken only by the occasional "
        "whistle of the wind. The ground is barren, with only the hardiest of plants "
        "struggling to survive in the harsh conditions."
    });

    wastes_long =
        "The wasteland opens up into a vast, open area, where the horizon seems to "
        "stretch endlessly. The ground is a patchwork of cracked earth and sparse "
        "vegetation, with the sun casting long shadows. In the distance, you can "
        "see the remnants of a ruined structure, a ghost of what once was, standing "
        "as a testament to the desolation that surrounds you.";
}

public void setup_short(object room, string file) {
    int x, y, z;
    string room_type;

    sscanf(file, "%d,%d,%d", x, y, z);
    room_type = determine_room_type(x, y);

    room->set_short(element_of(wastes_shorts));
}

public void setup_long(object room, string file) {
    int x, y, z;
    string room_type;

    sscanf(file, "%d,%d,%d", x, y, z);
    room_type = determine_room_type(x, y);

    room->set_long(element_of(wastes_longs));
}

public void setup_exits(object room) {
    int x, y, z;
    int max_x, max_y;
    string current_room_type;
    string room_type;
    mapping exits;
    string file = query_file_name(room);
    if(sscanf(file, "%d,%d,%d", x, y, z) != 3)
        return;

    // _debug("x: %d, y: %d, z: %d", x, y, z);

    max_x = get_map_width();
    max_y = get_map_height();

    current_room_type = determine_room_type(x, y);

    if(current_room_type == "impassable")
        return;

    // West
    if(x-1 >= 0) {
        room_type = determine_room_type(x-1, y);
        if(room_type != "impassable") {
            room->add_exit("west", sprintf("%d,%d,%d", x-1, y, z));
        }
    }

    // East
    if(x+1 < max_x) {
        room_type = determine_room_type(x+1, y);
        if(room_type != "impassable") {
            room->add_exit("east", sprintf("%d,%d,%d", x+1, y, z));
        }
        }

    // South
    if(y+1 < max_y) {
        room_type = determine_room_type(x, y+1);
        if(room_type != "impassable") {
            room->add_exit("south", sprintf("%d,%d,%d", x, y+1, z));
        }
    }

    // North
    if(y-1 >= 0) {
        room_type = determine_room_type(x, y-1);
        if(room_type != "impassable") {
            room->add_exit("north", sprintf("%d,%d,%d", x, y-1, z));
        }
    }

    // Northeast
    if(x+1 < max_x && y-1 >= 0) {
        room_type = determine_room_type(x+1, y-1);
        if(room_type != "impassable") {
            room->add_exit("northeast", sprintf("%d,%d,%d", x+1, y-1, z));
        }
    }

    // Northwest
    if(x-1 >= 0 && y-1 >= 0) {
        room_type = determine_room_type(x-1, y-1);
        if(room_type != "impassable") {
            room->add_exit("northwest", sprintf("%d,%d,%d", x-1, y-1, z));
        }
    }

    // Southeast
    if(x+1 < max_x && y+1 < max_y) {
        room_type = determine_room_type(x+1, y+1);
        if(room_type != "impassable") {
            room->add_exit("southeast", sprintf("%d,%d,%d", x+1, y+1, z));
        }
    }

    // Southwest
    if(x-1 >= 0 && y+1 < max_y) {
        room_type = determine_room_type(x-1, y+1);
        if(room_type != "impassable") {
            room->add_exit("southwest", sprintf("%d,%d,%d", x-1, y+1, z));
        }
    }

    // Southwest
    if(file == "0,0,0")
        room->add_exit("west", "../forest/27,11,0");
}

private nosave string *terrain_types = ({ "rocky", "sandy", "impassable","grassy" }) ;
private nosave mixed *thresholds = ({
    ({ 2, "impassable" }),   // 0 -  10% (10%)
    ({ 3, "sandy" }),   // 11 -  20% (10%)
    ({ 8, "rocky" }),   // 31 -  40% (10%)
    ({ 9, "impassable" }),   // 41 -  50% (10%)
});

string determine_room_type(int x, int y) {
    int noise_value = get_room_type(x, y);

    foreach(mixed threshold in thresholds) {
        int threshold_value = threshold[0];
        string terrain_type = threshold[1];

        if(noise_value <= threshold_value) {
            return terrain_type;
        }
    }

    return "unknown";
}

public void setup_room_type(object room) {
    string file = query_file_name(room);
    string room_type;
    int x, y, z;
    int noise_value;

    sscanf(file, "%d,%d,%d", x, y, z); // Extract coordinates from the file name

    room_type = determine_room_type(x, y);
    noise_value = get_room_type(x, y);

    // _debug("X: %d, Y: %d, Noise Value: %d, Terrain Type: %s", x, y, noise_value, room_type);

    room->set_room_environment(room_type);
}

void print_row(int y) {
    int x;
    mixed *map = query_noise_map_data();

    for(x = 0; x < sizeof(map); x++) {
        printf("%f ", map[x][y]);
    }
    printf("\n");
}
