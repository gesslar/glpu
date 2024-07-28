/**
 * @file /d/village/field_daemon.c
 * @description Daemon for handling the field stuff
 *
 * @created 2024/07/24 - Gesslar
 * @last_modified 2024/07/24 - Gesslar
 *
 * @history
 * 2024/07/24 - Gesslar - Created
 */

inherit STD_DAEMON ;

// Functions
private void setup_field_exits() ;
private void setup_field_shorts() ;
private void setup_field_longs() ;

// Variables
private nosave mapping field_exits = ([]) ;

void setup() {
    setup_field_exits() ;
    setup_field_shorts() ;
    setup_field_longs() ;
}

void setup_exits(object room, string file) {
    mapping exits ;

    if(!(exits = field_exits[file]))
        return ;

    room->set_exits(exits) ;
}

void setup_field_exits() {
    field_exits = ([
        "field1" : ([
            "north" : "field4",
            "east" : "field2",
            "south" : "../village_path1"
        ]),
        "field2" : ([
            "west" : "field1",
            "east" : "field3",
            "north" : "field5",
        ]),
        "field3" : ([
            "west" : "field2",
            "north" : "field6",
        ]),
        "field4" : ([
            "north" : "field7",
            "south" : "field1",
            "east" : "field5",
        ]),
        "field5" : ([
            "west" : "field4",
            "south" : "field2",
            "east" : "field6",
            "north" : "field8",
        ]),
        "field6" : ([
            "west" : "field5",
            "south" : "field3",
            "north" : "field9",
        ]),
        "field7" : ([
            "south" : "field4",
            "east" : "field8",
        ]),
        "field8" : ([
            "west" : "field7",
            "south" : "field5",
            "east" : "field9",
        ]),
        "field9" : ([
            "west" : "field8",
            "south" : "field6",
        ]),
    ]) ;
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
        "splashes of color to the verdant landscape. The air is fresh and "
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
