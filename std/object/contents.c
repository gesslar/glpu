// /std/object/contents.c
// Handles the contents of an object
//
// Created:     2024/02/18: Gesslar
// Last Change: 2024/02/18: Gesslar
//
// 2024/02/18: Gesslar - Created

#include <contents.h>
#include <gmcp_defines.h>
#include <daemons.h>
#include <weight.h>

#include "/std/living/include/wealth.h"

private int max_capacity ;
private nosave int capacity ;

void clean_contents() {
    object *obs = deep_inventory() ;

    obs->clean_contents() ;
    obs->remove() ;
}

void set_max_capacity(int x) {
    if(!mud_config("USE_MASS"))
        return ;

    max_capacity = x ;

    rehash_capacity() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity(),
            GMCP_LBL_CHAR_STATUS_MAX_CAPACITY : query_max_capacity()
        ])
    ) ;
}

void adjust_max_capacity(int x) {
    if(!mud_config("USE_MASS"))
        return ;

    max_capacity += x ;

    rehash_capacity() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity(),
            GMCP_LBL_CHAR_STATUS_MAX_CAPACITY : query_max_capacity()
        ])
    ) ;
}

int query_max_capacity() {
    if(!mud_config("USE_MASS"))
        return null ;

    return max_capacity ;
}

int query_capacity() {
    if(!mud_config("USE_MASS"))
        return null ;

    return capacity ;
}

int adjust_capacity(int x) {
    if(!mud_config("USE_MASS"))
        return null ;

    if (capacity + x < 0 || capacity + x > max_capacity) {
        return 0 ;
    }

    capacity += x ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity(),
            GMCP_LBL_CHAR_STATUS_MAX_CAPACITY : query_max_capacity()
        ])
    ) ;

    return 1 ;
}

void rehash_contents() {
    rehash_capacity() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity(),
            GMCP_LBL_CHAR_STATUS_MAX_CAPACITY : query_max_capacity(),
        ])
    ) ;
}

int can_hold_object(object ob) {
    int use_mass = mud_config("USE_MASS") ;
    int mass = ob->query_mass() ;

    if(use_mass)
        return can_hold_mass(mass) ;
    else
        return 1 ;
}

int can_hold_mass(int mass) {
    return capacity - mass >= 0 ;
}

void rehash_capacity() {
    object ob, *obs ;
    int total ;

    if(!mud_config("USE_MASS"))
        return ;

    total = 0 ;
    obs = all_inventory() ;
    foreach(ob in obs) {
        ob->rehash_capacity() ;
        total += ob->query_mass() ;
    }

    if(living())
        total += query_total_coins() ;

    capacity = max_capacity - total ;
}
