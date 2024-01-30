/* look.c

 Tacitus @ LPUniversity
 06-Oct-06
 Look Command (Rewrite # ... 3?)

*/

mixed render_room(object,object);
mixed render_object(object,object,string);
mixed render_container(object,object,string);
string highlight_view(string str, string *keys);

mixed main(object caller, object room, string arguments) {
    string target;

    room = environment(caller);

    if(!stringp(arguments)) return render_room(caller, room) ;
    if(sscanf(arguments, "at %s", target)) return render_object(caller, room, lower_case(target)) ;
    if(sscanf(arguments, "in %s", target)) return render_container(caller, room, lower_case(target)) ;
    else return render_object(caller, room, arguments) ;
}

string highlight_view(string str, string *keys) {
    int i;

    // TODO: Recode following with regexp?

    if(sizeof(keys) <= 0) return str;

    for(i = 0; i < sizeof(keys); i++) {
        str = replace_string(str , " " + keys[i] + " ", " \e<re1>" + keys[i] + "\e<res> ");
        str = replace_string(str , " " + capitalize(keys[i]) + " ", " \e<re1>" + capitalize(keys[i]) + "\e<res> ");
        str = replace_string(str , " " + keys[i] + ",", " \e<re1>" + keys[i] + "\e<res>,");
        str = replace_string(str , " " + keys[i] + ".", " \e<re1>" + keys[i] + "\e<res>.");
        str = replace_string(str , " " + keys[i] + "!", " \e<re1>" + keys[i] + "\e<res>!");
        str = replace_string(str , " " + keys[i] + ";", " \e<re1>" + keys[i] + "\e<res>;");
        str = replace_string(str , " " + keys[i] + "'", " \e<re1>" + keys[i] + "\e<res>'");
        str = replace_string(str , " " + keys[i] + ":", " \e<re1>" + keys[i] + "\e<res>:");
    }

    return str;
}

mixed render_room(object caller, object room) {
    string *exits, *users, *objects;
    string result = "" ;
    mixed data, datum ;

    if(!objectp(room))
        return "You see nothing because you have no environment!\n" ;

    data = get_short(room);
    if(data) result += data + "\n" ;
    data = get_long(room);
    if(data) result += highlight_view(data, keys(room->query_items())) + "\n" ;

    exits = keys(room->query_exits());

    switch(sizeof(exits)) {
        case 0 :
            result += "\nThere are no obvious exits here.\n";
            break;
        case 1 :
            result += "\nThe only obvious exit is " + exits[0] + ".\n";
            break;
        case 2 :
            result += "\nThe only obvious exits are " + implode(exits, " and ") + ".\n";
            break;
        default :
            result += "\nThe only obvious exits are " + implode(exits[0..<2], ", ") + ", and " + exits[<1] + ".\n";
            break;
    }

    users = filter(all_inventory(room), (: living($1) && $1 != $2 :), caller);
    objects = filter(all_inventory(room), (: !living($1) :));

    if(sizeof(users) > 0) {
        data = implode(map(users, (: get_short :)), "\n") ;
        result += data + "\n" ;
    }

    if(sizeof(objects) > 0) {
        data = implode(map(objects, (: get_short :)), "\n") ;
        result += data + "\n" ;
    }

    caller->tell(result);
    return 1 ;
}

mixed render_object(object caller, object room, string target) {
    object ob, user;
    string subtarget;

    if(stringp(room->query_item(target))) {
        caller->tell(highlight_view(room->query_item(target), keys(room->query_items())) + "\n");
        return 1;
    }

    if(target == "me") {
        ob = caller;
    } else if(sscanf(target, "%s in %s", target, subtarget) == 2 ||
        sscanf(target, "%s on %s", target, subtarget) == 2 && subtarget != "me") {
        user = present(subtarget, room);
        if(!objectp(user)) return "You do not see " + subtarget+ ".\n" ;
        ob = present(target, user);
    } else {
        ob = present(target, room);
        if(!objectp(ob)) ob = present(target, caller);
    }

    if(!objectp(ob)) return "You do not see " + target + ".\n" ;

    if(living(ob)) {
        object *inv = all_inventory(ob);

        if(ob == caller) {
            caller->tell("You look at yourself.\n");
            room->tell(caller->query_cap_name() + " looks at themselfs.\n", ({ ob }));

            caller->tell("\t\e<0015>" + ob->query_cap_name() + "\e<res>\n\n");
            caller->tell(ob->query_long() + "\n");
            caller->tell(implode(filter(inv->query("extra_look"), (: $1 :)),"\n") + "\n\n");

            if(sizeof(inv) <= 0) caller->tell("You are carrying nothing.\n\n");
            else caller->tell("You are carrying:\n" + implode(inv->query_short(), "\n") + "\n\n");
        } else {
            caller->tell("You look at " + ob->query_cap_name() + ".\n");
            room->tell(caller->query_cap_name() + " looks at " +ob->query_cap_name() + ".\n", ({ caller, ob }) );
            ob->tell(caller->query_cap_name() + " looks at you.\n");

            caller->tell("\t\e<00015>" + ob->query_cap_name() + "\e<res>\n\n");
            caller->tell(ob->query_long() + "\n");
            caller->tell(implode(filter(inv->query("extra_look"), (: $1 :)),"\n") + "\n\n");

            if(sizeof(inv) <= 0) caller->tell(ob->query_cap_name() + " is carrying nothing.\n\n");
            else caller->tell(ob->query_cap_name() + " is carrying:\n" + implode(inv->query_short(), "\n") + "\n\n");
        }
    } else {
        if(objectp(user)) {
            caller->tell("You look at a " + ob->query_short() + " on "+ user->query_cap_name() + "\n\n");
            user->tell(caller->query_cap_name() + " looks you over.\n");
            room->tell(caller->query_cap_name() +" looks over " + ob->query_short() + "\n", ({ caller, user }));
        } else {
            caller->tell("You look at a " + ob->query_short() + "\n\n");
            room->tell(caller->query_cap_name() + " looks at "+ ob->query_short() + "\n", ({ caller }) );
        }

        caller->tell(ob->query_long() + "\n");
    }

    return 1 ;
}

mixed render_container(object caller, object room, string target) {
    return "Error: Containers are not implemented yet.\n" ;
}

string help(object caller) {
    return(HIW + " SYNTAX:" + NOR + " look [<in/at>] <object> [<on/in> <object>]\n\n" +
      "This command will allow you to look at objects in your environment.\n" +
      "If no command is supplied, it will show you the whole room. You can\n" +
      "also look at specific objects by typing 'look <object>' or 'look at object'.\n" +
      "You might also try looking into an object by type 'look in <object>'.\n");
}
