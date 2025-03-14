/**
 * @file /std/area/equip/armour.c
 * @description More protective clothing!
 *
 * @created 2024-07-25 - Gesslar
 * @last_modified 2024-07-25 - Gesslar
 *
 * @history
 * 2024-07-25 - Gesslar - Created
 */

#include <armour.h>

inherit STD_CLOTHING;

private mapping _defense = ([ ]);
private float _ac = 0.0;

void mudlib_setup() {
    ::mudlib_setup();
    set_name("armour");
    set_short("armour");
    set_long("This is a piece of chest armour.");
    set_mass(1);
    set_defense(([
        "slashing" : 1.0,
        "piercing" : 1.0,
        "bludgeoning" : 1.0,
    ]));
    set_slot("torso");
}

void set_id(mixed ids) {
    ::set_id(ids);
    add_id(({ "armour", "armor" }));
}

int equip(object tp, string slot) {
    if(!::equip(tp, slot)) return 0;

    tp->adjust_protection();

    return 1;
}

int unequip(object tp, int silent) {
    if(!objectp(tp))
        tp = environment();

    if(!::unequip(tp))
        return 0;

    if(tp)
        tp->adjust_protection();

    return 1;
}

public void set_defense(mapping def) {
    _defense = def;
}

public void add_defense(string type, float amount) {
    if(!_defense) _defense = ([ ]);
    _defense[type] = amount;
}

public mapping query_defense() {
    return _defense;
}

public float query_defense_amount(string type) {
    if(!_defense) return 0.0;
    return _defense[type];
}

public void set_ac(float ac) {
    _ac = ac;
}

public float query_ac() {
    return _ac;
}

public float add_ac(float ac) {
    _ac += ac;
    return _ac;
}

int is_armour() { return 1; }
