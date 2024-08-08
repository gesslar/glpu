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
#include <container.h>

#include "/std/living/include/wealth.h"

private int _capacity ;
private nosave int _fill ;

void clean_contents() {
    object *obs = deep_inventory() ;

    obs->clean_contents() ;
    obs->remove() ;
}

void set_capacity(int x) {
    if(!mud_config("USE_MASS"))
        return ;

    _capacity = x ;

    rehash_capacity() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_FILL : query_fill(),
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity()
        ])
    ) ;
}

void adjust_capacity(int x) {
    if(!mud_config("USE_MASS"))
        return ;

    _capacity += x ;

    rehash_capacity() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_FILL : query_fill(),
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity()
        ])
    ) ;
}

int query_capacity() {
    if(!mud_config("USE_MASS"))
        return null ;

    return _capacity ;
}

int query_fill() {
    if(!mud_config("USE_MASS"))
        return null ;

    return _fill ;
}

int adjust_fill(int x) {
    if(!mud_config("USE_MASS"))
        return null ;

    if(_fill + x < 0 || _fill + x > _capacity)
        return 0 ;

    _fill += x ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_FILL : query_fill(),
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity()
        ])
    ) ;

    return 1 ;
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
    return _fill + mass <= _capacity ;
}

void rehash_capacity() {
    object ob, *obs ;
    int total ;

    if(!mud_config("USE_MASS"))
        return ;

    if(ignore_capacity())
        return ;

    total = 0 ;
    obs = all_inventory() ;
    foreach(ob in obs) {
        ob->rehash_capacity() ;
        total += ob->query_mass() ;
    }

    if(living())
        total += query_total_coins() ;

    _fill = total ;

    if(userp()) {
        GMCP_D->send_gmcp(this_object(),
            GMCP_PKG_CHAR_STATUS, ([
                GMCP_LBL_CHAR_STATUS_FILL : query_fill(),
                GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity()
            ])
        ) ;
    }
}
