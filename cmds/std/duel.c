// /cmds/std/duel.c
// This is the command to start combat.
//
// Created:     2024/02/20: Gesslar
// Last Change: 2024/02/20: Gesslar
//
// 2024/02/20: Gesslar - Created

inherit STD_ACT;

mixed main(object tp, string arg) {
    object victim;
    string name, vname;
    object room = environment(tp);

    if(!arg)
        return "Duel whom?";

    if(tp->is_dead())
        return "You can't duel while dead.";

    if(tp->is_ghost())
        return "You can't duel while a ghost.";

    if(!victim = find_target(tp, arg))
        return "You don't see that here.";

    if(victim->is_dead())
        return "You can't duel a dead person.";

    if(victim->is_ghost())
        return "You can't duel a ghost.";

    name = tp->query_name();
    vname = victim->query_name();

    tp->targetted_action("$N $vengage $t in combat.\n", victim);

    tp->start_attack(victim);

    return 1;
}
