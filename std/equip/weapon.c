/**
 * @file /std/equip/weapon.c
 * @description Standard weapong inherits
 *
 * @created 2024-08-04 - Gesslar
 * @last_modified 2024-08-04 - Gesslar
 *
 * @history
 * 2024-08-04 - Gesslar - Created
 */

#include <weapon.h>

inherit STD_ITEM ;

private nosave int hands = 1 ;
private nosave int equipped = 0 ;
private nosave string slot ;
private mixed dc = 1.0 ;
private string damage_type = "bludgeoning" ;

void set_hands(int i) { hands = i ; }
int query_hands() { return hands ; }

void set_slot(string str) {
    slot = str ;
}

void set_id(mixed ids) {
    ::set_id(ids) ;
    add_id(({ "weapon" })) ;
}

void set_dc(mixed x) {
    if(nullp(x))
        error("Bad argument 1 to set_dc().\n") ;

    if(valid_function(x))
        dc = x ;

    if(!floatp(x) && !intp(x))
        error("Bad argument 1 to set_dc().\n") ;

    dc = to_float(x) ;
}

float query_dc() {
    if(valid_function(dc))
        return evaluate(dc) ;

    return dc ;
}

void set_damage_type(string dt) {
    damage_type = dt ;
}

string query_damage_type() {
    return damage_type ;
}

string query_slot() {
    return slot ;
}

mixed can_equip(object tp) {
    return call_if(this_object(), "equip_check", tp) || 1 ;
}

mixed equip(object tp, string slot) {
    mixed result ;
    object env = environment() ;

    if(env != tp) {
        printf("Not in same environment: %O != %O\n", env, tp) ;
        return 0 ;
    }

    if(equipped) {
        printf("Already equipped: %O\n", this_object()) ;
        return 0 ;
    }

    if(tp->wielded_in(slot)) {
        printf("Already wielded in slot %s: %O\n", slot, tp->wielded_in(slot)) ;
        return 0 ;
    }

    result = tp->equip(this_object(), slot) ;
    if(result != 1)
        return result ;

    equipped = 1 ;
    return 1 ;
}

mixed can_unequip(object tp) {
    return call_if(this_object(), "unequip_check", tp) || 1 ;
}

varargs int unequip(object tp, int silent) {
    mixed result ;

    if(!equipped)
        return 0 ;

    if(!tp)
        tp = environment() ;

    if(tp) {
        if(tp->equipped(this_object()))
            return 0 ;

        if(!tp->unequip(this_object()))
            return 0 ;

        if(!silent) {
            tp->simple_action("$N $vunwield $o.", get_short()) ;
        }
    }

    equipped = 0 ;

    return 1 ;
}

int move(mixed dest) {
    object env = environment() ;
    int ret = ::move(dest) ;

    if(env)
        if(equipped)
            if(!ret)
                unequip(env) ;

    return ret ;
}

int equipped() {
    return equipped ;
}

void unsetup() {
    unequip(environment()) ;
}

int is_weapon() { return 1 ; }
