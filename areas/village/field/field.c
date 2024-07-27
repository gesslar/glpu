// /areas/village/field/field.c
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_ROOM ;

void virtual_start() {
    string file = create_args[0] ;

    set_zone("../olum") ;
    __DIR__ "field_daemon"->setup_exits(this_object(), file) ;
}

void setup() {
    add_object(__DIR__ "mon/boar") ;
}
