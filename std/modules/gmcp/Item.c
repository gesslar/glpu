#include <daemons.h>
#include <gmcp_defines.h>

inherit __DIR__ "gmcp_module" ;

void setup() {
    cooldown_limits = ([
        GMCP_PKG_ITEM_LIST : 60,
    ]) ;
}

void List(string target) {
    object prev = previous_object() ;
    object container ;

    // Core.Hello should only arrive from the login object at the start of the
    // connection.
    if(base_name(prev) == LOGIN_OB)
        return;

    if(target == "inventory")
        container = prev ;
    else if(target == "room")
        container = environment(prev) ;
    else
        return ;

    GMCP_D->send_gmcp(prev, GMCP_PKG_ITEM_LIST, container) ;
}
