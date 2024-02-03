// /std/daemon/daemon.c
// Base daemon inheritible
//
// Created:     2022/08/23: Gesslar
// Last Change: 2022/08/23: Gesslar
//
// 2022/08/23: Gesslar - Created

inherit M_CLEAN ;
inherit M_SETUP ;
inherit M_SAVE ;

// Functions
void remove() ;

// Private so only drivers can call it.
private void create() {
    setup_chain() ;
    if(query_persistent())
        restore_data() ;
}

void remove() {
    if(query_persistent())
        save_data() ;

    event(({ this_object() }), "remove") ;
}

int is_daemon() {
    return 1 ;
}
