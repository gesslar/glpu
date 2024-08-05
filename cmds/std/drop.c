/* drop.c

 Ico2 @ LPUniversity
 27-OCTOBER-05
 Standard command

QC Review: Tacitus on October 28th, 2005

Last edited on June 27th, 2006 by Tacitus

*/

inherit STD_CMD ;

mixed main(object tp, string arg) {
    object room = environment(tp);

    if(!arg) return(notify_fail("SYNTAX: drop <object>\n"));

    if(arg == "all") {
        object *inv = all_inventory(tp);

        if(!sizeof(inv))
            return "You don't have anything in your inventory.\n";

        foreach(object item in inv) {
            if(item->prevent_drop()) {
                tell(tp, "You cannot drop " + get_short(item) + ".\n") ;
                continue ;
            }

            if(item->move(room)) {
                tell(tp, "You could not drop " + get_short(item) + ".\n");
                continue ;
            }

            tp->simple_action("$N $vdrop $p $o.", get_short(item)) ;
        }
    } else if(sscanf(arg, "all %s", arg)) {
        object item, *inv ;

        inv = filter(all_inventory(tp), (: $1->id($(arg)) :));
        if(!sizeof(inv))
            return "You don't have any '" + arg + "' in your inventory.\n";

        foreach(item in inv) {
            if(item->move(room)) {
                if(item->id(arg)) {
                    tp->simple_action("$N $vdrop $p $o.", get_short(item)) ;
                }
            } else {
                tell(tp, "You could not drop " + get_short(item) + ".\n");
            }
        }
    } else {
        object ob;

        ob = present(arg, tp);

        if(!ob) {
            return "You don't have a '" + arg + "' in your inventory.\n";
        } else if(ob->prevent_drop()) {
            return "You cannot drop " + get_short(ob) + ".\n";
        } else {
            if(ob->move(room)) {
                return "You could not drop " + get_short(ob) + ".\n";
            } else {
                tp->simple_action("$N $vdrop $p $o.", get_short(ob)) ;
            }
        }
    }

    return 1;
}

string help(object tp)
{
     return(" SYNTAX: drop <item>\n\n"
     "This command will allow you to drop an object you are currently\n"
     "holding onto the ground. The argument you provide, will be the\n"
     "name of the object you wish to drop.\n\n" +
     "See also: get\n");
}
