// /std/modules/gmcp/gmcp_module.c
//
// Created:     2024/02/23: Gesslar
// Last Change: 2024/02/23: Gesslar
//
// 2024/02/23: Gesslar - Created

#include <gmcp.h>

inherit STD_DAEMON ;

protected nosave mapping cooldowns = ([]) ;
protected nosave mapping cooldown_limits = ([]) ;

int cooldown_check(object prev, string cooldown) {
    int limit = cooldown_limits[cooldown] ;
    int now = time() ;
    int next ;
    string cooldown_label = query_privs(prev) ;

    if(!limit)
        return 1 ;

    if(!cooldowns[cooldown])
        cooldowns[cooldown] = ([]) ;

    next = cooldowns[cooldown_label] ;
    if(next && next > now)
        return 0 ;

    return 1 ;
}

void apply_cooldown(object prev, string cooldown) {
    int limit = cooldown_limits[cooldown] ;
    int now = time() ;
    string cooldown_label = query_privs(prev) ;

    if(!limit)
        return ;

    if(!cooldowns[cooldown])
        cooldowns[cooldown] = ([]) ;

    cooldowns[cooldown_label] = now + limit ;
}
