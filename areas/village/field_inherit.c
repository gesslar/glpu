// /areas/village/field_inherit.c
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_ROOM ;

varargs void virtual_start() {
    string file = create_args[0] ;

    set_zone(__DIR__ "olum") ;
    __DIR__ "field"->setup_room(this_object(), file) ;
}
