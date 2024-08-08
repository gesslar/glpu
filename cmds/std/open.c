/**
 * @file /cmds/std/open.c
 * @description Command to open a container or door
 *
 * @created 2024/08/07 - Gesslar
 * @last_modified 2024/08/07 - Gesslar
 *
 * @history
 * 2024/08/07 - Gesslar - Created
 */

inherit STD_ACT ;

mixed main(object tp, string args) {
    string target;
    object source ;
    int here_flag ;
    object ob;

    if(!args)
        return "Open what?";

    if(sscanf(args, "%s here", target) == 1)
        here_flag = 1;
    else
        target = args;

    if(here_flag) {
        if(!ob = find_target(target, environment(tp)))
            return "You do not see that here.";
    } else {
        if(!ob = find_target(tp, target, tp))
            if(!ob = find_target(tp, target))
                return "You do not see " + target + " anywhere.";
    }

    if(!ob->closeable())
        return "You cannot open that.";

    if(!ob->is_closed())
        return "It is already open.";

    ob->set_closed(0);
    tp->simple_action("$N $vopen $o.\n", ob);

    return 1;
}
