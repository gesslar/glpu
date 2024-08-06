/* look.c

 Tacitus @ LPUniversity
 06-Oct-06
 Look Command (Rewrite # ... 3?)

*/

inherit STD_ACT ;

mixed render_room(object caller, object room, int brief);
mixed render_object(object caller, object room, string target);
mixed render_container(object caller, object room, string target);
mixed render_living(object caller, object room, object target, int brief);
string highlight_view(string str, string *keys);

private nosave string default_highlight_colour = mud_config("LOOK_HIGHLIGHT_COLOUR");
private nosave string look_hightlight_enabled = mud_config("LOOK_HIGHLIGHT");

mixed main(object caller, string arguments) {
    string target;
    object room = environment(caller);

    if(!stringp(arguments)) return render_room(caller, room, 0) ;
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

mixed render_room(object caller, object room, int brief) {
    string *exits, *users, *objects;
    string result = "" ;
    mixed data, datum ;

    if(!objectp(room))
        return "You see nothing because you have no environment!\n" ;

    if(devp(caller) && of(caller->query_env("look_filename"), ({ "on", "all" }))) {
        result += "\e0032\e"+room+"\eres\e" ;
        if(virtualp(room))
            result += " \e0068\e(vm: " + room->query_virtual_master() + ")\eres\e" ;

        result += "\n" ;
    }

    data = get_short(room);
    if(stringp(data) && strlen(data)) result += data + "\n" ;
    data = get_long(room);
    if(stringp(data) && strlen(data)) result += "\n" + highlight_view(caller, data, keys(room->query_items())) + "\n" ;

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
        string result = highlight_view(caller, room->query_item(target), keys(room->query_items())) ;
        result = append(result, "\n");
        tell(caller, result);
        tell_direct(room, name + " looks at " + target + ".\n", null, ({ caller }) );
        return 1;
    }

    if(target == "me") {
        ob = caller;
    } else {
        ob = present(target, room);
        if(!objectp(ob)) ob = present(target, caller);
    }

    if(!objectp(ob))
        return "You do not see " + target + ".\n" ;

    if(living(ob))
        return render_living(caller, room, ob, 0) ;

    temp = get_short(ob);
    if(stringp(temp)) desc += temp + "\n" ;
    temp = get_long(ob);
    if(stringp(temp)) desc += "\n" + temp + "\n" ;

    if(strlen(desc)) {
        if(devp(caller) && caller->query_env("look_filename") == "all") {
            desc = "\e0032\e"+prepend(file_name(ob), "/") + "\eres\e\n" + desc ;
        }
    }

    tell(ob, name + " looks at you.") ;
    tell_down(room, name + " looks at " + get_short(ob) + ".\n", null, ({ caller, ob }) );

    tell(caller, desc) ;

    return 1 ;
}

mixed render_living(object caller, object room, object target, int brief) {
    string temp, result = "" ;
    string name ;
    mapping equipment ;
    object ob ;
    string slot ;
    string race, gender, hair, eyes ;

    temp = get_short(target);
    if(stringp(temp))
        result += temp + "\n" ;
    temp = get_long(target);
    if(stringp(temp))
        if(strlen(result) && strlen(temp))
            temp += "\n" ;
        result += temp ;

    race = target->query_race() ;
    gender = target->query_gender() ;
    hair = target->query_hair_string() ;
    eyes = target->query_eyes_string() ;

    if(race) {
        if(gender) {
            if(gender == "male" || gender == "female") {
                temp = " a "+gender+" " + race ;
            } else {
                temp = " "+add_article(race) ;
            }
        } else {
            temp = " "+add_article(race) ;
        }
    } else {
        if(gender) {
            temp = gender ;
        } else {
            temp = "" ;
        }
    }

    if(hair && eyes && hair != "no hair" && eyes != "no eyes") {
        if(strlen(temp)) {
            temp += ", with " + hair + " and " + eyes ;
        } else {
            temp = hair + " and " + eyes ;
        }
    } else if(hair && hair != "no hair") {
        if(strlen(temp)) {
            temp += ", with " + hair ;
        } else {
            temp = hair ;
        }
    } else if(eyes && eyes != "no eyes") {
        if(strlen(temp)) {
            temp += ", with " + eyes ;
        } else {
            temp = eyes ;
        }
    }

    if(strlen(temp)) {
        if(caller == target)
            temp = "You are" + temp ;
        else
            temp = capitalize(subjective(target)) + " is" + temp ;
    }

    if(strlen(temp))
        temp = append(temp, ".") ;

    if(strlen(temp) && strlen(result))
    result += temp + "\n";

    name = caller->query_name() ;
    if(target == caller) {
        string refl = reflexive(caller) ;
        tell_down(room, name + " looks at "+refl+".\n", null, ({ caller }) );
    } else {
        string vname = target->query_name() ;
        tell(target, name + " looks at you.\n") ;
        tell_down(room, name + " looks at " + vname + ".\n", null, ({ caller, target }) );
    }

    if(strlen(result)) {
        if(devp(caller) && caller->query_env("look_filename") == "all") {
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

    tell(caller, result) ;
    return 1 ;
}

mixed render_container(object caller, object room, string target) {
    return "Error: Containers are not implemented yet.\n" ;
}

string query_help(object caller) {
    return(
"SYNTAX: look [<in/at>] <object> [<on/in> <object>]\n\n"
"This command will allow you to look at objects in your environment. "
"If no argument is supplied, it will show you the whole room. You can "
"also look at specific objects by typing 'look <object>' or 'look at object'. "
"You might also try looking into an object by typing 'look in <object>'.");
}
