// /adm/daemons/combat.c
// Combat routines
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

inherit STD_DAEMON ;

void start_combat(object attacker, object victim) {
    object combat ;

    if(!objectp(attacker) || !objectp(victim))
        return;

    if(!objectp(combat = find_ob(OBJ_COMBAT, attacker)))
        combat = new(OBJ_COMBAT);
    combat->start_combat(attacker, victim);

    if(!objectp(combat = find_ob(OBJ_COMBAT, victim)))
        combat = new(OBJ_COMBAT);
    combat->start_combat(victim, attacker);
}

object find_combat_object(object ob) {
    return find_ob(OBJ_COMBAT, ob);
}
