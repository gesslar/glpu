// /areas/village/virtual_area/field_inherit.c
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_ROOM ;

void virtual_start() {
    set_zone(__DIR__ "olum") ;
    __DIR__ "virtual_server"->setup_exits(this_object()) ;
}
