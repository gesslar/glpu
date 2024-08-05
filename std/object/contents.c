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
#include "/std/living/include/wealth.h"

private int max_capacity, max_volume ;
private nosave int capacity, volume ;

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

void set_max_volume(int x) {
    if(!mud_config("USE_BULK"))
        return ;

    max_volume = x ;

    rehash_volume() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_VOLUME : query_volume(),
            GMCP_LBL_CHAR_STATUS_MAX_VOLUME : query_max_volume()
        ])
    ) ;
}

void adjust_max_volume(int x) {
    if(!mud_config("USE_BULK"))
        return ;

    max_volume += x ;

    rehash_volume() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_VOLUME : query_volume(),
            GMCP_LBL_CHAR_STATUS_MAX_VOLUME : query_max_volume()
        ])
    ) ;
}

int query_max_capacity() {
    if(!mud_config("USE_MASS"))
        return null ;

    return max_capacity ;
}

int query_max_volume() {
    if(!mud_config("USE_BULK"))
        return null ;

    return max_volume ;
}

int query_capacity() {
    if(!mud_config("USE_MASS"))
        return null ;

    return capacity ;
}

int query_volume() {
    if(!mud_config("USE_BULK"))
        return null ;

    return volume ;
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

int adjust_volume(int x) {
    if(!mud_config("USE_BULK"))
        return null ;

    if (volume + x < 0 || volume + x > max_volume) {
        return 0 ;
    }
    volume += x ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_VOLUME : query_volume(),
            GMCP_LBL_CHAR_STATUS_MAX_VOLUME : query_max_volume()
        ])
    ) ;

    return 1 ;
}

void rehash_contents() {
    rehash_capacity() ;
    rehash_volume() ;

    GMCP_D->send_gmcp(this_object(),
        GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_CAPACITY : query_capacity(),
            GMCP_LBL_CHAR_STATUS_MAX_CAPACITY : query_max_capacity(),
            GMCP_LBL_CHAR_STATUS_VOLUME : query_volume(),
            GMCP_LBL_CHAR_STATUS_MAX_VOLUME : query_max_volume()
        ])
    ) ;
}

int can_hold_object(object ob) {
    int use_mass = mud_config("USE_MASS") ;
    int use_bulk = mud_config("USE_BULK") ;
    int mass = ob->query_mass() ;
    int bulk = ob->query_bulk() ;

    if(use_mass && use_bulk)
        return can_hold_mass(mass) && can_hold_bulk(bulk) ;
    else if(use_mass)
        return can_hold_mass(mass) ;
    else if(use_bulk)
        return can_hold_bulk(bulk) ;
    else
        return 1 ;
}

int can_hold_bulk(int bulk) {
    return volume - bulk >= 0 ;
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

void rehash_volume() {
    object ob, *obs ;
    int total ;

    if(!mud_config("USE_BULK"))
        return ;

    total = 0 ;
    obs = all_inventory() ;
    foreach(ob in obs) {
        ob->rehash_volume() ;
        total += ob->query_bulk() ;
    }

    if(living())
        total += query_total_coins() ;

    volume = max_volume - total ;
}
