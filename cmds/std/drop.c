/* drop.c

 Ico2 @ LPUniversity
 27-OCTOBER-05
 Standard command

QC Review: Tacitus on October 28th, 2005

Last edited on June 27th, 2006 by Tacitus

*/

inherit STD_CMD ;

mixed main(object caller, object room, string arg) {
    if(!arg) return(notify_fail("SYNTAX: drop <object>\n"));

    if(arg == "all") {
        object *inv = all_inventory(caller);

        if(!sizeof(inv))
            return "You don't have anything in your inventory.\n";

        foreach(object item in inv) {
            int result ;

            if(item->prevent_drop()) {
                tell(caller, "You cannot drop " + get_short(item) + ".\n") ;
                continue ;
            }

            if(!(result = item->move(room) & MOVE_OK)) {
                tell(caller, "You could not drop " + get_short(item) + ".\n");
                continue ;
            }

            tell(caller, "You drop " + get_short(item) + ".\n");
            tell_down(room, caller->query_name() + " drops " + get_short(item) + ".\n", 0, caller);
        }
    } else if(sscanf(arg, "all %s", arg)) {
        object item, *inv ;

        inv = filter(all_inventory(caller), (: $1->id($(arg)) :));
        if(!sizeof(inv))
            return "You don't have any '" + arg + "' in your inventory.\n";

        foreach(item in inv) {
            int result ;
            if(!(result = item->move(room) != MOVE_OK)) {
                if(item->id(arg)) {
                    tell(caller, "You drop " + get_short(item) + ".\n");
                    tell_down(room, caller->query_name() + " drops " + get_short(item) + ".\n", 0, caller);
                }
            } else {
                tell(caller, "You could not drop " + get_short(item) + ".\n");
            }
        }
    } else {
        object ob;

        ob = present(arg, caller);

        if(!ob) {
            return "You don't have a '" + arg + "' in your inventory.\n";
        } else if(ob->prevent_drop()) {
            return "You cannot drop " + get_short(ob) + ".\n";
        } else {
            int result ;
            if((result = ob->move(room) != MOVE_OK)) {
                return "You could not drop " + get_short(ob) + ".\n";
            } else {
                tell(caller, "You drop " + get_short(ob) + ".\n");
                tell_down(room, caller->query_name() + " drops " + get_short(ob) + ".\n", 0, caller);
            }
        }
    }

    return 1;
}

string help(object caller)
{
     return(" SYNTAX: drop <item>\n\n"
     "This command will allow you to drop an object you are currently\n"
     "holding onto the ground. The argument you provide, will be the\n"
     "name of the object you wish to drop.\n\n" +
     "See also: get\n");
}
