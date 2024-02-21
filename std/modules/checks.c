// /std/modules/checks.c
// A checks module.
//
// Created:     2024/02/20: Gesslar
// Last Change: 2024/02/20: Gesslar
//
// 2024/02/20: Gesslar - Created

varargs int same_room_check(object ob1, object ob2, int top_check) {
    object env1, env2;

    if(!objectp(ob1) || !objectp(ob2))
        return 0;

    if(top_check) {
        env1 = top_environment(ob1);
        env2 = top_environment(ob2);
    } else {
        env1 = environment(ob1);
        env2 = environment(ob2);
    }

    if(!objectp(env1) || !objectp(ob2))
        return 0;

    if(!env1->is_room() || !env2->is_room())
        return 0;

    return env1 == env2;
}
