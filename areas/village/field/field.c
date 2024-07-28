// /areas/village/field/field.c
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_ROOM ;

void virtual_setup(mixed args...) {
    string file = args[0] ;

    set_zone("../olum") ;

    __DIR__ "field_daemon"->setup_exits(this_object(), file) ;
    __DIR__ "field_daemon"->setup_short(this_object(), file) ;
    __DIR__ "field_daemon"->setup_long(this_object(), file) ;

    add_object("/mob/wild_boar") ;
}
