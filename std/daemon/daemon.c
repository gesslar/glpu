// /std/daemon/daemon.c
// Base daemon inheritible
//
// Created:     2022/08/23: Gesslar
// Last Change: 2022/08/23: Gesslar
//
// 2022/08/23: Gesslar - Created

inherit M_CLEAN ;
inherit M_SETUP ;

// Functions
void remove() ;

protected void create() {
    setup_chain() ;
}

void remove() {
    event(({ this_object() }), "remove") ;
}
