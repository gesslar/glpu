/* look.c

 Tacitus @ LPUniversity
 06-Oct-06
 Look Command (Rewrite # ... 3?)

*/

mixed render_room(object,object);
mixed render_object(object,object,string);
mixed render_container(object,object,string);
string highlight_view(string str, string *keys);

private nosave string default_highlight_colour = "0243" ;

mixed main(object caller, object room, string arguments) {
    string target;

    room = environment(caller);

    if(!stringp(arguments)) return render_room(caller, room) ;
    if(sscanf(arguments, "at %s", target)) return render_object(caller, room, lower_case(target)) ;
    if(sscanf(arguments, "in %s", target)) return render_container(caller, room, lower_case(target)) ;
    else return render_object(caller, room, arguments) ;
}

string highlight_view(object tp, string str, string *keys) {
    int i;
    string colour ;

    // TODO: Recode following with regexp?

    if(sizeof(keys) <= 0) return str;
    if(!colour = tp->query_env("highlight"))
        colour = default_highlight_colour;
    else
        colour = "\e<" + colour + ">";

    for(i = 0; i < sizeof(keys); i++) {
        str = replace_string(str , " " + keys[i] + " ", " " + colour+keys[i] + "\e<res> ");
        str = replace_string(str , " " + capitalize(keys[i]) + " ", " " + colour+capitalize(keys[i]) + "\e<res> ");
        str = replace_string(str , " " + keys[i] + ",", " " + colour+keys[i] + "\e<res>,");
        str = replace_string(str , " " + keys[i] + ".", " " + colour+keys[i] + "\e<res>.");
        str = replace_string(str , " " + keys[i] + "!", " " + colour+keys[i] + "\e<res>!");
        str = replace_string(str , " " + keys[i] + ";", " " + colour+keys[i] + "\e<res>;");
        str = replace_string(str , " " + keys[i] + "'", " " + colour+keys[i] + "\e<res>'");
        str = replace_string(str , " " + keys[i] + ":", " " + colour+keys[i] + "\e<res>:");
    }

    return str;
}

mixed render_room(object caller, object room) {
    string *exits, *users, *objects;
    string result = "" ;
    mixed data, datum ;

    if(!objectp(room))
        return "You see nothing because you have no environment!\n" ;

    if(devp(caller) && caller->query_env("room_filename") == "on") {
        result += "\e<0032>"+prepend(file_name(room), "/") + "\e<res>\n" ;
    }

    data = get_short(room);
    if(data) result += data + "\n" ;
    data = get_long(room);
    if(data) result += "\n" + highlight_view(caller, data, keys(room->query_items())) + "\n" ;

    exits = keys(room->query_exits());

    switch(sizeof(exits)) {
        case 0 :
            data = "There are no obvious exits here.\n";
            break;
        case 1 :
            data = "You may go " + exits[0] + ".\n";
            break;
        case 2 :
            data = "You may go " + implode(exits, " or ") + ".\n";
            break;
        default :
            data = "You may go " + implode(exits[0..<2], ", ") + ", or " + exits[<1] + ".\n";
            break;
    }
    if(data) result += "\n" + data ;

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

    tell(caller, result) ;
    return 1 ;
}

mixed render_object(object caller, object room, string target) {
    object ob, user;
    string subtarget;

    if(stringp(room->query_item(target))) {
        tell(caller, highlight_view(caller, room->query_item(target), keys(room->query_items())) + "\n");
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
            tell(caller, "You look at yourself.\n");
            tell_down(room, caller->query_cap_name() + " looks at themself.\n", UNDEFINED, ({ caller }) );

            tell(caller, "\t\e<0015>" + ob->query_cap_name() + "\e<res>\n\n");
            tell(caller, get_long(ob) + "\n");

            if(sizeof(inv) <= 0) tell(caller, "You are carrying nothing.\n\n");
            else tell(caller, "You are carrying:\n" + implode(map(inv, (: get_short :)), "\n") + "\n\n");
        } else {
            tell(caller, "You look at " + ob->query_cap_name() + ".\n");
            tell(ob, caller->query_cap_name() + " looks at you.\n");
            tell_down(room, caller->query_cap_name() + " looks at " + ob->query_cap_name() + ".\n", UNDEFINED, ({ caller, ob }) );

            tell(caller, "\t\e<0015>" + ob->query_cap_name() + "\e<res>\n\n");
            tell(caller, get_long(ob) + "\n");

            if(sizeof(inv) <= 0) tell(caller, ob->query_cap_name() + " is carrying nothing.\n\n");
            else tell(caller, ob->query_cap_name() + " is carrying:\n" + implode(map(inv, (: get_short :)), "\n") + "\n\n");
        }
    } else {
        if(objectp(user)) {
            tell(caller, "You look at a " + get_short(ob) + " on " + user->query_cap_name() + ".\n");
            tell(user, caller->query_cap_name() + " looks at a " + get_short(ob) + " on you.\n");
            tell_down(room, caller->query_cap_name() + " looks at a " + get_short(ob) + " on " + user->query_cap_name() + ".\n", ({ caller, user }) );
        } else {
            tell(caller, "You look at a " + get_short(ob) + ".\n");
            tell_down(room, caller->query_cap_name() + " looks at a " + get_short(ob) + ".\n", UNDEFINED, ({ caller }) );
        }

        tell(caller, get_long(ob) + "\n");
    }

    return 1 ;
}

mixed render_container(object caller, object room, string target) {
    return "Error: Containers are not implemented yet.\n" ;
}

string help(object caller) {
    return("SYNTAX: look [<in/at>] <object> [<on/in> <object>]\n\n" +
        "This command will allow you to look at objects in your environment.\n" +
        "If no command is supplied, it will show you the whole room. You can\n" +
        "also look at specific objects by typing 'look <object>' or 'look at object'.\n" +
        "You might also try looking into an object by type 'look in <object>'.\n");
}
