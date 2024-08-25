// /std/equip/equip.c
// Basic equipment object
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

#include <clothing.h>
#include <armour.h>
#include <gmcp_defines.h>

inherit STD_ITEM ;

void set_slot(string str) ;
string query_slot() ;
private mapping _slots = ([ ]) ;
private nosave string _slot ;
private nosave int _equipped = 0 ;

mixed can_equip(string slot, object tp) {
    return call_if(this_object(), "equip_check", slot, tp) || 1 ;
}

void set_slot(string str) {
    _slot = str ;
}

string query_slot() {
    return _slot ;
}

mixed equip(object tp, string slot) {
    mixed result ;
    object env = environment() ;

    if(env != tp)
        return 0 ;

    if(_equipped)
        return 0 ;

    if(tp->equipped_on(slot))
        return 0 ;

    result = tp->equip(this_object(), slot) ;
    if(result != 1)
        return result ;

    _equipped = 1 ;

    GMCP_D->send_gmcp(tp, GMCP_PKG_CHAR_ITEMS_UPDATE, ({ this_object(), tp })) ;

    return 1 ;
}

mixed can_unequip(object tp) {
    return call_if(this_object(), "unequip_check", tp) || 1 ;
}

varargs int unequip(object tp, int silent) {
    mixed result ;

    if(!_equipped)
        return 0 ;

    if(!tp)
        tp = environment() ;

    if(tp) {
        if(tp->equipped_on(query_slot()) != this_object())
            return 0 ;

        if(!tp->unequip(this_object()))
            return 0 ;

        if(!silent) {
            tp->simple_action("$N $vremove $p $o.", get_short()) ;
        }
    }

    _equipped = 0 ;

    GMCP_D->send_gmcp(tp, GMCP_PKG_CHAR_ITEMS_UPDATE, ({ this_object(), tp })) ;

    return 1 ;
}

int move(mixed dest) {
    object env = environment() ;
    int ret = ::move(dest) ;

    if(env)
        if(_equipped)
            if(!ret)
                unequip(env) ;

    return ret ;
}

int equipped() { return _equipped ; }

void unsetup() {
    unequip(environment()) ;
}
