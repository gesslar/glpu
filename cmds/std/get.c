/* get.c

 Ico2 @ LPUniversity
 27-OCTOBER-05
 Standard Command

QC Review: Tacitus on October 28th, 2005

Last edited on March 24th, 2006 by Tacitus

*/

inherit STD_CMD ;

mixed main(object caller, object room, string arg) {
    object ob;

    if(!arg)
        return "SYNTAX: get <object|all|all <id>>\n";

    if(arg == "all") {
        object *obs;
        int i, sz ;

        obs = all_inventory(room);
        obs = filter(obs, (: !living($1) :) );
        obs = filter(obs, (: !($1->prevent_get()) :) );

        if(!sz = sizeof(obs))
            return "There is nothing to get.\n";

        for(i = 0; i < sz; i++) {
            if(obs[i]->move(caller) & MOVE_OK) {
                tell(caller, "You get "+get_short(obs[i])+".\n");
                tell_down(room, caller->query_name() + " gets "+get_short(obs[i])+".\n", 0, caller);
            }
        }
    } else if(sscanf(arg, "all %s", arg)) {
        object *obs;
        int i, sz;

        obs = filter(all_inventory(room), (: $1->id($2) :) , arg);
        obs = filter(obs, (: !living($1) :) );
        obs = filter(obs, (: !($1->prevent_get()) :) );

        if(!sz = sizeof(obs))
            return "There is nothing to get.\n";

        for(i = 0; i < sz; i++) {
            if(obs[i]->move(caller) & MOVE_OK) {
                tell(caller, "You get "+get_short(obs[i])+".\n");
                tell_down(room, caller->query_name() + " gets "+get_short(obs[i])+".\n", 0, caller);
            }
        }
    } else {
        ob = present(arg, room);

        if(!ob)
            return "You see no '" + arg + "' here.\n";

        if(ob->prevent_get())
            return get_short(ob) + " cannot be picked up.\n";

        if(!(ob->move(caller) & MOVE_OK))
            return "You were unable to pick up "+get_short(ob)+".\n";

        tell(caller, "You get "+get_short(ob)+".\n");
        tell_down(room, caller->query_name() + " gets "+get_short(ob)+".\n", 0, caller);
    }

    return 1;
}

string help(object caller) {
    return(" SYNTAX: get <item>\n\n"
        "This command will allow you to get an object from your current\n"
        "environment. The argument you provide, will be the name of the\n"
        "object you wish to drop.\n\n" +
        "See also: drop\n");
}
