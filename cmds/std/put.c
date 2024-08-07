/**
 * @file /cmds/std/put.c
 * @description Command to put an object into a container
 *
 * @created 2024/08/07 - Gesslar
 * @last_modified 2024/08/07 - Gesslar
 *
 * @history
 * 2024/08/07 - Gesslar - Created
 */

inherit STD_ACT ;

mixed main(object tp, string str) {
    string dest ;
    object env, ob, container ;

    if(!str)
        return "Put what where?" ;

    if(sscanf(str, "%s in %s", str, dest) != 2)
        return "Put what where?" ;

    env = environment(tp) ;

    if(!ob = find_carried_object(tp, str))
        return "You do not have "+add_article(str)+"." ;

    if(!container = find_carried_object(tp, dest)) {
        if(!container = find_local_target(tp, dest))
            return 1 ;
    }

    if(!container->is_container())
        return get_short(container) + " is not a container." ;

    if(ob == container)
        return "You cannot put " + get_short(ob) + " in itself." ;

    if(ob->prevent_drop() || ob->prevent_put() || container->prevent_put())
        return "You cannot put " + get_short(ob) + " in " + get_short(container) + "." ;

    if(!container->can_hold_object(ob))
        return get_short(container) + " cannot hold " + get_short(ob) + "." ;

    if(ob->move(container))
        return "You could not put " + get_short(ob) + " in " + get_short(container) + "." ;

    tp->simple_action("$N $vput a $o into a $o1.", ob, container) ;

    return 1 ;
}
