/**
 * @file /std/user/include/equipment.c
 * @description Equipment system for livings
 *
 * @created 2024/07/29 - Gesslar
 * @last_modified 2024/07/29 - Gesslar
 *
 * @history
 * 2024/07/29 - Gesslar - Created
 */

#include <body.h>

protected nosave mapping equipment = ([ ]) ;
public mapping query_equipped() { return copy(equipment); }
public object equipped_on(string slot) { return equipment[slot] || null ; }
mixed can_equip(string slot, object ob) ;

int equip(string slot, object ob) {
    if(!of(slot, query_body_slots()))
        return 0 ;

    if(equipment[slot])
        return 0 ;

    equipment[slot] = ob ;

    return 1 ;
}

int unequip(string slot) {
    if(!equipment[slot])
        return 0 ;

    map_delete(equipment, slot) ;

    return 1 ;
}

mixed can_equip(string slot, object ob) {
    if(!of(slot, query_body_slots()))
        return "Your body cannot wear something in there." ;

    if(equipment[slot])
        return "You are already wearing something like that." ;

    return 1 ;
}
