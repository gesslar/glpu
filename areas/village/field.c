// /areas/village/field.c
// Virtual server for the Olum zone
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_DAEMON ;

void setup_field_exits() ;
mapping field_exits = ([]) ;

void setup() {
    setup_field_exits() ;
}

void setup_room(object room, string file) {
    _debug("Setting up room: %O, file: %O", room, file) ;
    if(of(file, field_exits)) {
        mapping exits = copy(field_exits[file]) ;
        string direction, exit ;

        room->set_exits(exits) ;
        _debug("Setting up room: " + file) ;
        _debug("Exits: %O", room->query_exits()) ;
    }

}

void setup_field_exits() {
    // An exit beginning with a # is a literal exit, not a virtual one
    // The setup_exits() function will handle these differently
    field_exits = ([
        "field1" : ([
            "north" : "field4",
            "east" : "field2",
            "south" : "/areas/village/village_path1"
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
