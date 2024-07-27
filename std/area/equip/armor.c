/**
 * @file /std/area/equip/armor.c
 * @description More protective clothing!
 *
 * @created 2024/07/25 - Gesslar
 * @last_modified 2024/07/25 - Gesslar
 *
 * @history
 * 2024/07/25 - Gesslar - Created
 */

inherit STD_CLOTHING ;

private mapping defense = ([ ]) ;

void mudlib_setup() {
    set_name("armor") ;
    set_short("armor") ;
    set_long("This is a piece of chest armor.") ;
    set_weight(1) ;
    set_defense(([
        "slashing" : 1.0,
        "piercing" : 1.0,
        "bludgeoning" : 1.0,
    ])) ;
    set_slots(([
        "torso" : 100.0,
    ])) ;
}

public void set_defense(mapping def) {
    defense = def ;
}

public void add_defense(string type, float amount) {
    if(!defense) defense = ([ ]) ;
    defense[type] = amount ;
}

public mapping query_defense() {
    return defense ;
}

public float query_defense_amount(string type) {
    if(!defense) return 0.0 ;
    return defense[type] ;
}

int is_armor() { return 1; }
