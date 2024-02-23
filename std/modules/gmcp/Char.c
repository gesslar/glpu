#include <daemons.h>
#include <gmcp.h>

inherit __DIR__ "gmcp_module" ;

void setup() {
    cooldown_limits = ([
        GMCP_PKG_CHAR_STATUS : 60,
    ]) ;
}

void Status() {
    object prev = previous_object();

    if(!cooldown_check(prev, GMCP_PKG_CHAR_STATUS))
        return;

    apply_cooldown(prev, GMCP_PKG_CHAR_STATUS);

    GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_STATUS) ;
}
