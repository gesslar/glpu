/**
 * @file /std/user/advancement.c
 * @description Advancement object for players
 *
 * @created 2024/07/24 - Gesslar
 * @last_modified 2024/07/24 - Gesslar
 *
 * @history
 * 2024/07/24 - Gesslar - Created
 */

#include <advancement.h>
#include <gmcp.h>

private float level = 1.0 ;
private float level_mod = 0.0 ;
private int xp = 0 ;

int query_xp() {
    return xp ;
}

float query_tnl() {
    return ADVANCE_D->tnl(level) ;
}

float query_level() {
    return level ;
}

float query_effective_level() {
    return level + level_mod ;
}

float set_level(float l) {
    level = to_float(l) ;

    if(userp()) {
        GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_XP: xp,
            GMCP_LBL_CHAR_STATUS_TNL: query_tnl(),
            GMCP_LBL_CHAR_STATUS_LEVEL: level,
        ])) ;
    }

    return level ;
}

float add_level(float l) {
    level += to_float(l) ;

    if(userp()) {
        GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_XP: xp,
            GMCP_LBL_CHAR_STATUS_TNL: query_tnl(),
            GMCP_LBL_CHAR_STATUS_LEVEL: level,
        ])) ;
    }

    return level ;
}

float query_level_mod() {
    return level_mod ;
}

float set_level_mod(float l) {
    level_mod = to_float(l) ;
    return level_mod ;
}

float add_level_mod(float l) {
    level_mod += to_float(l) ;
    return level_mod ;
}

int add_xp(int amount) {
    xp += amount ;

    if(userp()) {
        GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_XP: xp,
            GMCP_LBL_CHAR_STATUS_TNL: query_tnl(),
            GMCP_LBL_CHAR_STATUS_LEVEL: level,
        ])) ;
    }

    return xp ;
}

int set_xp(int amount) {
    xp = amount ;

    if(userp()) {
        GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_XP: xp,
            GMCP_LBL_CHAR_STATUS_TNL: query_tnl(),
            GMCP_LBL_CHAR_STATUS_LEVEL: level,
        ])) ;
    }

    return xp ;
}

void on_advance(object tp, float l) {
    tell(tp, "You have advanced to level " + to_int(l) + "!\n") ;
}
