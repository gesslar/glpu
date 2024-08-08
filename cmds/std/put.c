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
    string dest, target;
    object env, ob, container;
    int all_flag = 0;
    int here_flag = 0;

    if (!str)
        return "Put what where?";

    if (sscanf(str, "all %s in %s here", target, dest) == 2) {
        all_flag = 1;
        here_flag = 1;
    } else if (sscanf(str, "all %s in %s", target, dest) == 2) {
        all_flag = 1;
    } else if (sscanf(str, "%s in %s here", target, dest) == 2) {
        here_flag = 1;
    } else if (sscanf(str, "%s in %s", target, dest) != 2) {
        return "Put what where?";
    }

    env = environment(tp);

    // Find the container
    if (here_flag) {
        container = present(dest, env);
    } else {
        container = present(dest, tp);
        if (!container) {
            container = present(dest, env);
        }
    }

    if (!container)
        return "There is no container named '" + dest + "' here.";

    if (!container->is_container())
        return get_short(container) + " is not a container.";

    // Handle 'put all' scenarios
    if (all_flag) {
        object *obs = filter(all_inventory(tp), (: $1->id($2) :), target);
        int sz = sizeof(obs);
        int put_count = 0;

        if (!sz)
            return "You do not have any " + target + ".";

        foreach (object item in obs) {
            if (item == container)
                continue;

            if (item->prevent_drop() || item->prevent_put() || container->prevent_put())
                continue;

            if (!container->can_hold_object(item))
                continue;

            if (item->move(container)) {
                tell(tp, "You could not put " + get_short(item) + " in " + get_short(container) + ".\n");
                continue;
            }

            tp->simple_action("$N $vput a $o into a $o1.", item, container);
            put_count++;
        }

        if (put_count == 0)
            return "You could not put any " + target + " in " + get_short(container) + ".";

    } else {
        // Handle 'put <object>' scenarios
        if (!(ob = find_carried_object(tp, target)))
            return "You do not have " + add_article(target) + ".";

        if (ob == container)
            return "You cannot put " + get_short(ob) + " in itself.";

        if (ob->prevent_drop() || ob->prevent_put() || container->prevent_put())
            return "You cannot put " + get_short(ob) + " in " + get_short(container) + ".";

        if (!container->can_hold_object(ob))
            return get_short(container) + " cannot hold " + get_short(ob) + ".";

        if (ob->move(container))
            return "You could not put " + get_short(ob) + " in " + get_short(container) + ".";

        tp->simple_action("$N $vput a $o into a $o1.", ob, container);
    }

    return 1;
}
