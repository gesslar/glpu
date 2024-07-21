//goto.c

//Tacitus @ LPUniversity
//29-JUNE-05
//Wizard goto command
//   * Thanks to Icoz

//Last edited on July 22nd, 2006 by Parthenon

inherit STD_CMD ;

mixed main(object tp, string arg) {
    string custom, tmp;
    object ob, target, env, my_env;
    string file;

    if(!arg) return(notify_fail("Error [goto]: Where would you like to go?\n"));

    my_env = environment(tp);

    if(target = find_player(arg)) {
        if((env = environment(target)) == my_env)
            return "Error [goto]: You are already with " + target->cap_name() + "." ;

        if(tp->query_env("teleport_out") && wizardp(tp)) {
            custom = tp->query_env("teleport_out");

            tmp = custom;
            tmp = replace_string(tmp, "$N", tp->query_name());
            tmp = replace_string(tmp, "$D", target->query_name());

            tell_down(my_env, capitalize(tmp), 0, tp);
        } else {
            tell_down(my_env, tp->query_name() + " teleports to another location.", 0, ({ tp }) );
        }

        tp->move_living(env);

        if(tp->query_env("teleport_in") && wizardp(tp)) {
            custom = tp->query_env("teleport_in");

            tmp = custom;
            tmp = replace_string(tmp, "$N", tp->query_name());
            tmp = replace_string(tmp, "$D", "here");

            tell_down(env, capitalize(tmp), 0, tp);
        } else {
            tell_down(env, tp->query_name() + " has teleported to this room.", 0, tp);
        }

        tell(tp, "You have teleported to " + target->cap_name() + " in " + env->query_short() + "\n");

        return 1;
    } else {
        file = resolve_path(tp->query_env("cwd"), arg);

        file = append( file, ".c") ;
        if(!file_exists(file)) return "Error [goto]: " + file + " does not exist.";
        if(!ob = find_object(file)) ob = load_object(file);
        if(!ob) return "Error [goto]: Unable to move to " + file + ".\n";
        if(my_env == ob) return "Error [goto]: You are already there.\n";

        if(tp->query_env("teleport_out") && wizardp(tp)) {
            custom = tp->query_env("teleport_out");

            tmp = custom;
            tmp = replace_string(tmp, "$D", get_short(ob));
            tmp = replace_string(tmp, "$N", tp->query_name());

            tell_down(my_env, capitalize(tmp), 0, tp);
        } else {
            tell_down(my_env, tp->query_name() + " teleports to another location.", 0, ({ tp }) );
        }

        tp->move_living(ob);

        if(tp->query_env("teleport_in") && wizardp()) {
            custom = tp->query_env("teleport_in");

            tmp = custom;
            tmp = replace_string(tmp, "$D", "here");
            tmp = replace_string(tmp, "$N", tp->query_name());

            tell_down(ob, capitalize(tmp), 0, tp);
        } else {
            tell_down(ob, tp->query_name() + " has teleported to this room.", 0, tp);
        }

        tell(tp, "You have teleported to " + capitalize(get_short(ob)) + "\n");

        return 1;
    }

    return 0;
}

string help(object tp) {
    return (" SYNTAX: goto <room/user>" + "\n\n" +
      "The goto command allows you to transport to another room by\n"
      "filename or transport to another use. If ther user or room\n"
      "specified is not found, you will not be moved.\n");
}
