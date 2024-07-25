// /cmds/std/duel.c
// This is the command to start combat.
//
// Created:     2024/02/20: Gesslar
// Last Change: 2024/02/20: Gesslar
//
// 2024/02/20: Gesslar - Created

inherit STD_ACT ;

void setup() {
    target(TARGET_LOCAL) ;
}

mixed main(object tp, string arg) {
    object victim ;
    string name, vname ;
    object room = environment(tp) ;

    if (!arg)
        return "Duel whom?" ;

    if(!victim = find_local_target(tp, arg))
        return 1 ;

    name = tp->query_name() ;
    vname = victim->query_name() ;

    tell(tp, "You engage " + vname + " in combat.\n") ;
    tell(victim, name + " engages you in combat.\n") ;
    tell_down(room, name + " engages " + vname + " in combat.\n", 0,
        ({ tp, victim }) ) ;

    tp->start_attack(victim) ;

    return 1;
}
