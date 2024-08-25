/**
 * @file /d/village/field_daemon.c
 * @description Daemon for handling the field stuff
 *
 * @created 2024-07-24 - Gesslar
 * @last_modified 2024-07-24 - Gesslar
 *
 * @history
 * 2024-07-24 - Gesslar - Created
 */

inherit STD_DAEMON ;

// Functions
private void setup_field_shorts() ;
private void setup_field_longs() ;

// Variables
private nosave mapping field_exits = ([]) ;

void setup() {
    setup_field_shorts() ;
    setup_field_longs() ;
}

void setup_exits(object room) {
    int x, y, z;
    mapping exits = ([]);
    string file;

    file = query_file_name(room);
    if (sscanf(file, "%d,%d,%d", x, y, z) != 3) {
        return;
    }

    // Check and add exits for each direction
    if (x > 0) exits["west"] = sprintf("%d,%d,%d", x-1, y, z);
    if (x < 9) exits["east"] = sprintf("%d,%d,%d", x+1, y, z);
    if (y < 9) exits["south"] = sprintf("%d,%d,%d", x, y+1, z);
    if (y > 0) exits["north"] = sprintf("%d,%d,%d", x, y-1, z);

    // Special case for the bottom-left corner (0,9,0)
    if (file == "0,9,0") {
        exits["south"] = "../village_path1";
    }

    room->set_exits(exits);
}

private nosave string *field_shorts ;

string setup_field_shorts() {
    field_shorts = ({
        "Open Field",
        "Grassy Meadow",
        "Quiet Field",
        "Grassy Plains",
    }) ;
}

void setup_short(object room, string file) {
    room->set_short(element_of(field_shorts)) ;
}

private nosave int rot = 0 ;
private nosave string *field_longs ;

string setup_field_longs() {
    field_longs = ({
        "North of the village, an open grassy field spreads out, the tall "
        "blades of grass swaying gently in the breeze. The field is a vibrant "
        "green, with the occasional wildflower peeking through, adding "
        "splashes of colour to the verdant landscape. The air is fresh and "
        "carries the faint scent of earth and grass, creating a serene and "
        "peaceful atmosphere. This expansive field invites travelers to "
        "wander and enjoy the natural beauty that lies just beyond the "
        "village.",

        "Just north of the village, a sunny grassy meadow extends into the "
        "distance, the grass soft and lush underfoot. The meadow is bathed in "
        "warm sunlight, casting a golden hue over the landscape. The gentle "
        "rustling of the grass in the breeze is the only sound that breaks "
        "the tranquil silence. This open space offers a perfect retreat from "
        "the bustle of village life, a place to pause and appreciate the "
        "simple beauty of the natural world.",

        "To the north of the village, a quiet grassy expanse unfolds, its "
        "soft green grass dotted with tiny, delicate flowers. The area is "
        "peaceful, with the gentle hum of insects and the occasional birdsong "
        "filling the air. The grass sways rhythmically with the wind, "
        "creating a soothing and calming effect. This tranquil field serves "
        "as a serene escape, a place to relax and take in the beauty of the "
        "countryside.",

        "Northward from the village, wide grassy plains stretch as far as the "
        "eye can see, the grass tall and undisturbed. The plains are a sea of "
        "green, with the occasional gust of wind sending ripples through the "
        "grass. The open sky above adds to the sense of vastness and freedom, "
        "making it easy to lose oneself in the natural splendor of the "
        "landscape. This open field is an ideal spot for those seeking "
        "solitude and a connection with nature.",
    }) ;
}
void setup_long(object room, string file) {
    room->set_long(field_longs[rot]) ;
    if(++rot == sizeof(field_longs))
        rot = 0 ;
}
