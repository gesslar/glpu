// /std/equip/equip.c
// Basic equipment object
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_ITEM ;

void set_slot(string str) ;
string query_slot() ;
private mapping slots = ([ ]) ;
private nosave string slot ;
private nosave int equipped = 0 ;

mixed can_equip(string slot, object tp) {
    return call_if(this_object(), "equip_check", slot, tp) || 1 ;
}

void set_slot(string str) {
    slot = str ;
}

string query_slot() {
    return slot ;
}

mixed equip(string slot, object tp) {
    mixed result ;
    object env = environment() ;

    if(env != tp)
        return 0 ;

    if(equipped)
        return 0 ;

    if(tp->equipped_on(slot))
        return 0 ;

    result = tp->equip(slot, this_object()) ;
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

    if(tp) {
        if(tp->equipped_on(query_slot()) != this_object())
            return 0 ;

        if(!tp->unequip(query_slot()))
            return 0 ;

        if(!silent) {
            tell_object(tp, "You remove the "+get_short()+".\n") ;
            tell_down(environment(tp), tp->query_name()+" removes "+tp->query_possessive()+" "+get_short()+".\n", 0, tp) ;
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
            if(ret == MOVE_OK)
                unequip(env) ;

    return ret ;
}
