// /std/modules/targeting.c
// This file contains the code for the targeting livings, objects, etc
// This file must be inherited by any object that needs to target something.
//
// Created:     2024/02/20: Gesslar
// Last Change: 2024/02/20: Gesslar
//
// 2024/02/20: Gesslar - Created

#include <daemons.h>

private nosave string target_flags = "" ;

protected varargs void target(mixed args...) {
    int i ;

    if(!args || sizeof(args) < 1)
        return ;

    args = filter(args, (: intp :)) ;

    while(i--) {
        if(test_bit(target_flags, args[i]))
            clear_bit(target_flags, args[i]) ;
        else
            set_bit(target_flags, args[i]) ;
    }
}

varargs object find_local_target(object tp, string arg, function f) {
    object target, *obs, ob ;
    object room ;

    if(nullp(tp))
        error("Missing argument 1 for find_local_target");

    room = top_environment(tp);

    if(environment(tp) != room)
        return 0 ;

    if(!room) {
        tell(tp, "You are in a void, you can't target anything here.");
        return 0 ;
    }
#if 0
    if(!arg && test_bit(target_flags, TARGET_ATTACKER)) {
        object combat = COMBAT_D->find_combat_object(tp) ;

        if(combat) {
            object victim = combat->query_target() ;

            if(victim)
                return victim ;
        }
    }
#endif
    obs = all_inventory(room) ;
    if(test_bit(target_flags, TARGET_LIVING)) {
        object *tmp = ({}) ;
        obs = filter(obs, (: living($1) :)) ;

        if(test_bit(target_flags, TARGET_PLAYER))
            tmp += filter(obs, (: $1->is_pc() :)) ;

        if(test_bit(target_flags, TARGET_NPC))
            tmp += filter(obs, (: !$1->is_pc() :)) ;

        obs = tmp ;
    } else if(test_bit(target_flags, TARGET_OBJECT)) {
        obs = filter(obs, (: !living($1) :)) ;
    }

    if(test_bit(target_flags, TARGET_SELF)) {
        obs = filter(obs, (: $1 != $(tp) :)) ;
    }

    obs = filter(obs, (: $1->id($(arg)) :)) ;

    if(sizeof(obs) > 1) {
        tell(tp, "You need to be more specific.") ;
        return 0 ;
    }

    target = obs[0] ;

    if(!objectp(target)) {
        tell(tp, "You don't see that here.") ;
        return 0 ;
    }

    return target ;
}

varargs object find_carried_object(object tp, string arg, function f) {
    object *obs, ob ;
    string *matches ;
    int num ;

    if(nullp(tp))
        error("Missing argument 1 for find_carried_object");

    if(nullp(arg))
        error("Missing argument 2 for find_carried_object");

    obs = all_inventory(tp) ;
    matches = pcre_extract(arg, "^(.+?)(?: (\\d+))?$") ;
    if(!sizeof(matches))
        return 0 ;

    if(sizeof(matches) > 1) {
        arg = matches[0] ;
        num = to_int(matches[1]) - 1 ;
    } else {
        num = 0 ;
    }

    obs = filter(obs, (: $1->id($(arg)) :)) ;
    obs = filter(obs, (: $(tp)->can_see($1) :)) ;

    if(sizeof(obs) < 1)
        return 0 ;

    if(num > sizeof(obs) - 1)
        return 0 ;

    return obs[num] ;
}
