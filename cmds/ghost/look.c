/* look.c

 Tacitus @ LPUniversity
 06-Oct-06
 Look Command (Rewrite # ... 3?)

*/

inherit STD_ACT ;

mixed render_room(object tp, object room, int brief);
mixed render_object(object tp, object room, string target);
mixed render_container(object tp, object room, string target);
mixed render_living(object tp, object room, object target, int brief);
string highlight_view(string str, string *keys);

private nosave string default_highlight_colour = mud_config("LOOK_HIGHLIGHT_COLOUR");
private nosave string look_hightlight_enabled = mud_config("LOOK_HIGHLIGHT");

mixed main(object tp, string arguments) {
    string target;
    object room = environment(tp);

    if(!stringp(arguments)) return render_room(tp, room, 0) ;
    if(sscanf(arguments, "at %s", target)) return render_object(tp, room, lower_case(target)) ;
    if(sscanf(arguments, "in %s", target)) return render_container(tp, room, lower_case(target)) ;
    else return render_object(tp, room, arguments) ;
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
    if(sscanf(colour, "%d", i) != 1)
        colour = default_highlight_colour;
    else {
        if(i < 0 || i > 256) {
            colour = default_highlight_colour;
        } else {
            colour = sprintf("%'0'4d", i);
        }
    }

    colour = COLOUR_D->substitute_too_dark(colour);
    colour = "\e" + colour + "\e";

    for(i = 0; i < sizeof(keys); i++) {
        str = replace_string(str , " " + keys[i] + " ", " " + colour+keys[i] + "\eres\e ");
        str = replace_string(str , " " + capitalize(keys[i]) + " ", " " + colour+capitalize(keys[i]) + "\eres\e ");
        str = replace_string(str , " " + keys[i] + ",", " " + colour+keys[i] + "\eres\e,");
        str = replace_string(str , " " + keys[i] + ".", " " + colour+keys[i] + "\eres\e.");
        str = replace_string(str , " " + keys[i] + "!", " " + colour+keys[i] + "\eres\e!");
        str = replace_string(str , " " + keys[i] + ";", " " + colour+keys[i] + "\eres\e;");
        str = replace_string(str , " " + keys[i] + "'", " " + colour+keys[i] + "\eres\e'");
        str = replace_string(str , " " + keys[i] + ":", " " + colour+keys[i] + "\eres\e:");
    }

    return str;
}

mixed render_room(object tp, object room, int brief) {
    string *exits, *users, *objects;
    string result = "" ;
    mixed data, datum ;

    if(!objectp(room))
        return "You see nothing because you have no environment!\n" ;

    if(devp(tp) && of(tp->query_env("look_filename"), ({ "on", "all" }))) {
        result += "\e0032\e"+room+"\eres\e" ;
        if(virtualp(room))
            result += " \e0068\e(vm: " + room->query_virtual_master() + ")\eres\e" ;

        result += "\n" ;
    }

    data = get_short(room);
    if(stringp(data) && strlen(data)) result += data + "\n" ;
    data = get_long(room);
    if(stringp(data) && strlen(data)) result += "\n" + highlight_view(tp, data, keys(room->query_items())) + "\n" ;

    exits = keys(room->query_exits());

    switch(sizeof(exits)) {
        case 0 :
            data = "There are no obvious exits here.";
            break;
        case 1 :
            data = "You may go " + exits[0] + ".";
            break;
        case 2 :
            data = "You may go " + implode(exits, " or ") + ".";
            break;
        default :
            data = "You may go " + implode(exits[0..<2], ", ") + ", or " + exits[<1] + ".";
            break;
    }
    if(data) result += "\n" + data + "\n" ;

    users = filter(all_inventory(room), (: living($1) && $1 != $2 :), tp);
    objects = filter(all_inventory(room), (: !living($1) :));

    if(sizeof(users) > 0) {
        data = implode(map(users, (: get_short :)), "\n") ;
        result += data + "\n" ;
    }

    if(sizeof(objects) > 0) {
        data = implode(map(objects, (: get_short :)), "\n") ;
        result += data + "\n" ;
    }

    tell(tp, result) ;
    return 1 ;
}

mixed render_object(object tp, object room, string target) {
    object ob, user;
    string subtarget;
    string desc = "" ;
    string temp ;
    string name ;

    name = tp->query_name();

    if(stringp(room->query_item(target))) {
        string result = highlight_view(tp, room->query_item(target), keys(room->query_items())) ;
        result = append(result, "\n");
        tell(tp, result);
        tell_down(room, name + " looks at " + target + ".", null, ({ tp }) );
        return 1;
    }

    if(target == "me") {
        ob = tp;
    } else {
        if(!objectp(ob = find_target(tp, target))) {
            if(!objectp(ob = find_target(tp, target, tp))) {
                return "You do not see " + target + ".\n" ;
            }
        }
    }

    if(living(ob)) return render_living(tp, room, ob, 0) ;

    temp = get_short(ob);
    if(stringp(temp)) desc += temp + "\n" ;
    temp = get_long(ob);
    if(stringp(temp)) desc += "\n" + temp + "\n" ;


    if(strlen(desc)) {
        if(devp(tp) && tp->query_env("look_filename") == "all") {
            desc = "\e0032\e"+prepend(file_name(ob), "/") + "\eres\e\n" + desc ;
        }
    }

    tell(ob, name + " looks at you.") ;
    tell_down(room, name + " looks at " + ob->query_short() + ".", null, ({ tp, ob }) );

    tell(tp, desc) ;

    return 1 ;
}

mixed render_living(object tp, object room, object target, int brief) {
    string temp, result = "" ;
    string name ;
    mapping equipment ;
    object ob ;
    string slot ;

    if(stringp(temp = get_short(target))) result += temp + "\n" ;
    if(stringp(temp = get_long(target))) result += "\n" + temp + "\n" ;

    name = tp->query_name() ;
    if(target == tp) {
        string refl = reflexive(tp) ;
        tell_down(room, name + " looks at "+refl+".\n", null, ({ tp }) );
    } else {
        string vname = target->query_name() ;
        tell(target, name + " looks at you.\n") ;
        tell_down(room, name + " looks at " + vname + ".\n", null, ({ tp, target }) );
    }

    if(strlen(result)) {
        if(devp(tp) && tp->query_env("look_filename") == "all") {
            result = "\e0032\e"+prepend(file_name(target), "/") + "\eres\e\n" + result ;
        }
    }

    equipment = target->query_equipped() ;
    if(sizeof(equipment)) {
        string *slots = keys(equipment) ;
        int max = max(map(slots, (: strlen :))) ;

        result += "\n" ;
        foreach(slot, ob in equipment) {
            result += sprintf("%*s : %s\n", max, capitalize(slot), get_short(ob)) ;
        }
    }

    tell(tp, result) ;
    return 1 ;
}

mixed render_container(object tp, object room, string target) {
    return "Error: Containers are not implemented yet.\n" ;
}

string query_help(object tp) {
    return(
"SYNTAX: look [<in/at>] <object> [<on/in> <object>]\n\n"
"This command will allow you to look at objects in your environment. "
"If no argument is supplied, it will show you the whole room. You can "
"also look at specific objects by typing 'look <object>' or 'look at object'. "
"You might also try looking into an object by typing 'look in <object>'.");
}
