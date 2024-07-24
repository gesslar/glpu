// /std/daemon/daemon.c
// Base daemon inheritible
//
// Created:     2022/08/23: Gesslar
// Last Change: 2022/08/23: Gesslar
//
// 2022/08/23: Gesslar - Created

inherit STD_OBJECT ;
inherit M_SAVE ;

int is_daemon() {
    return 1 ;
}
