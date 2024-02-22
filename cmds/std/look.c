/* look.c

 Tacitus @ LPUniversity
 06-Oct-06
 Look Command (Rewrite # ... 3?)

*/

inherit STD_ACT ;

mixed render_room(object,object);
mixed render_object(object,object,string);
mixed render_container(object,object,string);
mixed render_living(object,object,object);
string highlight_view(string str, string *keys);

private nosave string default_highlight_colour = mud_config("LOOK_HIGHLIGHT_COLOUR");
private nosave string look_hightlight_enabled = mud_config("LOOK_HIGHLIGHT");

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

    if(look_hightlight_enabled != "on") return str;

    if(tp->query_env("highlight") != "on") return str;

    if(sizeof(keys) <= 0) return str;
    if(!colour = tp->query_env("highlight_colour"))
        colour = default_highlight_colour;

    // need to determine if number is from 0-256 with a leading 0
    if(!pcre_match(colour, "^0(?:1?[0-9]{1,2}|2[0-4][0-9]|25[0-5])$"))
        colour = "";
    else
        colour = "\e<" + colour + ">";

    colour = XTERM256->substitute_too_dark(colour);

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
    if(stringp(data) && strlen(data)) result += data + "\n" ;
    data = get_long(room);
    if(stringp(data) && strlen(data)) result += "\n" + highlight_view(caller,
        data, keys(room->query_items())) + "\n" ;

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
    if(data) result += "\n" + data + "\n";

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
    string desc = "" ;
    string temp ;
    string name ;

    name = caller->query_name();

    if(stringp(room->query_item(target))) {
        tell(caller, highlight_view(caller, room->query_item(target), keys(room->query_items())) + "\n");
        tell_down(room, name + " looks at " + target + ".", UNDEFINED, ({ caller }) );
        return 1;
    }

    if(target == "me") {
        ob = caller;
    } else {
        ob = present(target, room);
        if(!objectp(ob)) ob = present(target, caller);
    }

    if(!objectp(ob)) return "You do not see " + target + ".\n" ;

    if(living(ob)) return render_living(caller, room, ob) ;

    temp = get_short(ob);
    if(stringp(temp)) desc += temp + "\n" ;
    temp = get_long(ob);
    if(stringp(temp)) desc += "\n" + temp + "\n" ;

    tell(ob, name + " looks at you.") ;
    tell_down(room, name + " looks at " + ob->query_short() + ".", UNDEFINED, ({ caller, ob }) );

    tell(caller, desc) ;

    return 1 ;
}

mixed render_living(object caller, object room, object target, object user) {
    object *inv;
    string temp, result = "" ;
    string name ;

    if(stringp(temp = get_short(target))) result += temp + "\n" ;
    if(stringp(temp = get_long(target))) result += "\n" + temp + "\n" ;

    name = caller->query_cap_name() ;
    if(target == caller) {
        string refl = reflexive(caller) ;
        tell_down(room, name + " looks at "+refl+".\n", UNDEFINED, ({ caller }) );
    } else {
        string vname = target->query_cap_name() ;
        tell(target, name + " looks at you.\n") ;
        tell_down(room, name + " looks at " + vname + ".\n", UNDEFINED, ({ caller, target }) );
    }

    tell(caller, result) ;
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
