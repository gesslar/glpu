// /std/daemon/daemon.c
// Base daemon inheritible
//
// Created:     2022/08/23: Gesslar
// Last Change: 2022/08/23: Gesslar
//
// 2022/08/23: Gesslar - Created

inherit STD_OBJECT ;

inherit M_SETUP ;
inherit M_SAVE ;

// Private so only drivers can call it.
private void create() {
    setup_chain() ;
    if(query_persistent())
        restore_data() ;
}

int remove() {
    if(query_persistent())
        save_data() ;

    return ::remove() ;
}

int is_daemon() {
    return 1 ;
}
