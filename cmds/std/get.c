/* get.c

 Ico2 @ LPUniversity
 27-OCTOBER-05
 Standard Command

QC Review: Tacitus on October 28th, 2005

Last edited on March 24th, 2006 by Tacitus

*/

inherit STD_CMD ;

mixed main(object tp, string arg) {
    object ob ;
    object room = environment(tp);

    if(!arg)
        return "SYNTAX: get <object|all|all <id>>\n";

    if(arg == "all") {
        object *obs;
        int i, sz ;
        int got ;

        obs = all_inventory(room);
        obs = filter(obs, (: !living($1) :) );
        obs = filter(obs, (: !($1->prevent_get()) :) );

        if(!sz = sizeof(obs))
            return "There is nothing to get.\n";

        for(i = 0; i < sz; i++) {
            ob = obs[i] ;

            if(ob->move(tp)) {
                tell(tp, "You were unable to pick up "+get_short(ob)+".\n");
                continue;
            }

            tp->simple_action("$N $vpick up $o.", get_short(obs[i])) ;
            got = 1 ;
        }

        if(!got)
            return "You picked up nothing.\n";
    } else if(sscanf(arg, "all %s", arg)) {
        object *obs;
        int i, sz;
        int got ;

        obs = filter(all_inventory(room), (: $1->id($2) :) , arg);
        obs = filter(obs, (: !living($1) :) );
        obs = filter(obs, (: !($1->prevent_get()) :) );

        if(!sz = sizeof(obs))
            return "There is nothing to get.\n";

        for(i = 0; i < sz; i++) {
            ob = obs[i] ;

            if(ob->move(tp)) {
                tell(tp, "You were unable to pick up "+get_short(ob)+".\n");
                continue;
            }

            tp->simple_action("$N $vpick up $o.", get_short(obs[i])) ;
            got = 1 ;
        }

        if(!got)
            return "You picked up nothing.\n";
    } else {
        ob = present(arg, room);

        if(!ob)
            return "You see no '" + arg + "' here.\n";

        if(living(ob))
            return "You cannot pick up living things.\n";

        if(ob->prevent_get())
            return get_short(ob) + " cannot be picked up.\n";

        if(ob->move(tp))
            return "You were unable to pick up "+get_short(ob)+".\n";

        tp->simple_action("$N $vpick up $o.", get_short(ob)) ;
    }

    return 1;
}

string help(object tp) {
    return(" SYNTAX: get <item>\n\n"
        "This command will allow you to get an object from your current\n"
        "environment. The argument you provide, will be the name of the\n"
        "object you wish to drop.\n\n" +
        "See also: drop\n");
}
