// /adm/daemons/boot.c
// The boot daemon.
//
// Created:     2024/03/05: Gesslar
// Last Change: 2024/03/05: Gesslar
//
// 2024/03/05: Gesslar - Created

#include <origin.h>

inherit STD_DAEMON ;

private nomask int boot_number ;

void setup() {
    set_persistent(1) ;
}

void event_boot(object prev) {
    if(previous_object() != master())
        return ;
    debugf("Boot #%d loaded.", ++boot_number) ;
    save_data() ;
}

int query_boot_number() {
    return boot_number ;
}
