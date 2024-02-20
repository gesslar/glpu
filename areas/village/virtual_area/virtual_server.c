// /areas/village/virtual_area/virtual_server.c
// Virtual server for the Olum zone
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_VIRTUAL_SERVER ;

void setup_field_exits() ;
mapping field_exits = ([]) ;

void setup() {
    setup_field_exits() ;
}

object compile_object(string file) {
    object result ;
    mapping info = virtual_info(file) ;

    if(info["base"] == "field") {
        result = new(__DIR__ "field_inherit") ;
        return result ;
    }

    return 0 ;
}

void setup_exits(object room) {
    string file_name = file_name(room) ;
    mapping info = virtual_info(file_name) ;

    if(info["base"] == "field") {
        string file = info["file"][0] ;
        mapping exits = copy(field_exits[file]) ;
        string direction, exit ;
debug(json_encode(exits)) ;
        room->set_exits(exits) ;
    }
}

void setup_field_exits() {
    // An exit beginning with a # is a literal exit, not a virtual one
    // The setup_exits() function will handle these differently
    field_exits = ([
        "field1" : ([
            "north" : "field4",
            "east" : "field2",
            "south" : "/areas/village/virtual_area/village_path1"
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
