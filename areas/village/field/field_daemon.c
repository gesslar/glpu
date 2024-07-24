/**
 * @file /areas/village/field_daemon.c
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
public void setup_field_exits() ;

// Variables
private nosave mapping field_exits = ([]) ;

void setup() {
    setup_field_exits() ;
}

void setup_exits(object room, string file) {
    mapping exits ;

    if(!(exits = field_exits[file]))
        return ;

    room->set_exits(exits) ;
}

void setup_field_exits() {
    // An exit beginning with a # is a literal exit, not a virtual one
    // The setup_exits() function will handle these differently
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
