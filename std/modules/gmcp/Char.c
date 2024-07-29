#include <daemons.h>
#include <gmcp_defines.h>

inherit __DIR__ "gmcp_module" ;

void setup() {
    cooldown_limits = ([
        GMCP_PKG_CHAR_STATUS : 60,
        GMCP_PKG_CHAR_STATUSVARS: 60,
    ]) ;
}

void StatusVars() {
    object prev = previous_object();

    if(!cooldown_check(GMCP_PKG_CHAR_STATUSVARS, prev))
        return;

    apply_cooldown(GMCP_PKG_CHAR_STATUSVARS, prev);

    GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_STATUSVARS) ;
}

void Status() {
    object prev = previous_object();

    if(!cooldown_check(GMCP_PKG_CHAR_STATUS, prev))
        return;

    apply_cooldown(GMCP_PKG_CHAR_STATUS, prev);

    GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_STATUS) ;
}
