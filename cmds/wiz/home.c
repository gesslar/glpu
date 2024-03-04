/* home.c

Tacitus @ LPUniversity
23-OCT-05
Standard Wizard Command

This command will move you to ~/workroom.c if it
exists. You can also provide the name of another
developer to attempt to move into their workroom.

*/

//Last edited on July 22nd, 2006 by Parthenon

inherit STD_CMD ;

mixed main(object tp, object room, string str) {
    string custom, tmp;

    if(!str) {
        if(file_exists(user_path(tp->query_proper_name()) + "workroom.c")) {
            if(tp->query_env("teleport_out") && wizardp(tp)) {
                custom = tp->query_env("teleport_out");

                tmp = custom;
                tmp = replace_string(tmp, "$N", tp->query_name());
                tmp = replace_string(tmp, "$D", "home");

                tell_down(room, capitalize(tmp) + "\n", 0, tp);
            } else {
                tell_room(environment(tp), tp->query_proper_name() + " teleports home.\n", ({tp}));
            }

            tp->move_living(user_path(tp->query_proper_name()) + "workroom.c");

            room = environment(tp);
            if(tp->query_env("teleport_in") && wizardp(tp)) {
                custom = tp->query_env("teleport_in");

                tmp = custom;
                tmp = replace_string(tmp, "$N", tp->query_name());
                tmp = replace_string(tmp, "$D", "here");

                tell_down(room, capitalize(tmp) + "\n", 0, tp);
            } else {
                tell_down(room, tp->query_name() + " teleports here.\n", 0, tp);
            }

            tell(tp, "You teleport to your workroom.\n");
            return 1;
        } else {
            return "Error [home]: You do not have a workroom.\n";
        }
    } else {
        str = lower_case(str);
        if(file_exists(user_path(str) + "workroom.c")) {
            if(tp->query_env("teleport_out")) {
                custom = tp->query_env("teleport_out");

                tmp = custom;
                tmp = replace_string(tmp, "$N", tp->query_name());
                tmp = replace_string(tmp, "$D", capitalize(str) + "'s workroom.");

                tell_down(room, capitalize(tmp) + "\n", 0, tp);

                return 1;
            } else {
                tell_down(room, tp->query_name() + " teleports to someone's workroom.\n", 0, tp);
            }

            tp->move_living(user_path(str) + "workroom.c");

            room = environment(tp);
            if(tp->query_env("teleport_in") && wizardp(tp)) {
                custom = tp->query_env("teleport_in");

                tmp = custom;
                tmp = replace_string(tmp, "$N", tp->query_name());
                tmp = replace_string(tmp, "$D", "here");

                tell_down(room, capitalize(tmp) + "\n", 0, tp);
            } else {
                tell_down(room, tp->query_name() + " teleports here.\n", 0, tp);
            }

            tell(tp, "You teleported to " + capitalize(str) + "'s workroom.\n");

            return 1;
        } else {
            return "Error [home]: " + capitalize(str) + " does not have a workroom.\n";
        }
    }
}

string help(object tp)
{
    return("SYNTAX: workroom [<user>]\n\n"
      "This command will move you to ~/workroom.c if it\n"
      "exists. You can also provide the name of another\n"
      "developer to attempt to move into their workroom.\n");
}
