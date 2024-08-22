/**
 * @file /std/user/include/equipment.c
 * @description Equipment system for livings
 *
 * @created 2024-07-29 - Gesslar
 * @last_modified 2024-07-29 - Gesslar
 *
 * @history
 * 2024-07-29 - Gesslar - Created
 */

#include <body.h>

protected nosave mapping equipment = ([ ]) ;
protected nosave mapping wielded = ([ ]) ;
public mapping query_equipped() { return copy(equipment); }
public mapping query_wielded() { return copy(wielded); }
public object equipped_on(string slot) { return equipment[slot] || null ; }
public object wielded_in(string slot) { return wielded[slot] || null ; }

int equip(object ob, string slot) {
    string type ;

    if(ob->is_weapon())
        type = "weapon" ;
    else if(ob->is_armour())
        type = "armour" ;
    else if(ob->is_clothing())
        type = "clothing" ;
    else
        return 0 ;

    switch(type) {
        case "weapon": {
            int slots = ob->query_hands() ;
            mapping w = filter(wielded, (: !objectp($2) :)) ;
            string *all_weapon_slots = query_weapon_slots() ;
            int sz = sizeof(all_weapon_slots) ;

            if(!of(slot, all_weapon_slots)) {
                printf("Slot %s not found in %O\n", slot, all_weapon_slots) ;
                return 0 ;
            }
            if(wielded[slot]) {
                printf("Slot %s already occupied by %O\n", slot, wielded[slot]) ;
                return 0 ;
            }
            if(slots > (sz - sizeof(w))) {
                printf("Slots required: %d\n", slots) ;
                printf("Size of w: %d\n", sizeof(w)) ;
                printf("Not enough hands to wield %O\n", ob) ;
                printf("Occupied slots: %O\n", w) ;
                return 0 ;
            }
            wielded[slot] = ob ;
            all_weapon_slots -= keys(wielded) ;
            if(slots > 1) {
                while(--slots) {
                    string s = element_of(all_weapon_slots) ;
                    wielded[s] = ob ;
                    all_weapon_slots -= ({ s }) ;
                }
            }
            break ;
        }
        case "armour": case "clothing" :
            if(!of(slot, query_body_slots()))
                return 0 ;
            if(equipment[slot])
                return 0 ;
            equipment[slot] = ob ;
            break ;
    }

    return 1 ;
}

mixed equipped(object ob) {
    string *s ;

    if(!ob)
        return 0 ;

    if(ob->is_weapon()) {
        s = filter(keys(wielded), (: $2 == $(ob) :)) ;
        if(sizeof(s))
            return s ;
    } else {
        s = filter(keys(equipment), (: $2 == $(ob) :)) ;
        if(sizeof(s))
            return s[0] ;
    }

    return 0 ;
}

int unequip(mixed ob) {
    mixed slot ;
    mapping m ;

    if(objectp(ob)) {
        if(ob->is_weapon()) {
            m = wielded ;
            m = filter(m, (: $2 != $(ob) :)) ;
            wielded = m ;
        } else {
            m = equipment ;
            m = filter(m, (: $2 != $(ob) :)) ;
            equipment = m ;
        }

        return 1 ;
    } else if(stringp(ob)) {
        slot = ob ;
        if(of(slot, wielded)) {
            return unequip(wielded[slot]) ;
        } else if(of(slot, equipment)) {
            return unequip(equipment[slot]) ;
        } else {
            return 0 ;
        }
    }

    return 0 ;
}

mixed can_equip(mixed ob, string slot) {
    if(!objectp(ob))
        return 0 ;

    if(objectp(ob)) {
        if(ob->is_weapon()) {
            int hands = ob->query_hands() ;
            string *all_weapon_slots = query_weapon_slots() ;
            mapping w = filter(wielded, (: objectp($2) :)) ;
            int remaining_slots = sizeof(all_weapon_slots) - sizeof(w) ;
            if(!of(slot, all_weapon_slots))
                return "You cannot wield that there." ;
            if(wielded[slot])
                return "You are already wielding something in that hand." ;
            if(remaining_slots < hands)
                return "You do not have enough hands to wield that." ;
            return 1 ;
        }
    }

    if(stringp(ob))
        slot = ob ;
    else
        return 0 ;

    if(!of(slot, query_body_slots()))
        return "Your body cannot wear something in there." ;

    if(equipment[slot])
        return "You are already wearing something like that." ;

    return 1 ;
}
