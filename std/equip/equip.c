// /std/equip/equip.c
// Basic equipment object
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_OBJECT ;

private nosave string slot ;
private nosave string equipped = 0 ;

void set_slot(string str) {
    slot = str ;
}

string query_slot() {
    return slot ;
}

mixed equip(object tp, string slot) {
    object env = environment() ;
    string *slots ;

    if(env != tp)
        return 0 ;

    if (equipped)
        return 0 ;

    if (env->query_equipment(slot))
        return 0 ;

    slots = env->query_body_slots() ;
    if(member_array(slot, slots) == -1)
        return 0 ;

    equipped = slot ;
    return 1 ;
}

mixed unequip() {
    object env = environment() ;

    if (equipped) {
        equipped = 0 ;
    }

    return 1 ;
}

int move(mixed dest) {
    object env ;
    int ret = ::move(dest) ;

    env = environment() ;
    if(env)
        if(equipped)
            if(ret == MOVE_OK)
                unequip() ;

    return ret ;
}
