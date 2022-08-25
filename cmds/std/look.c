/* look.c

 Tacitus @ LPUniversity
 06-Oct-06 
 Look Command (Rewrite # ... 3?)
 
*/

int render_room(object,object);
int render_object(object,object,string);
int render_container(object,object,string);
string highlight_view(string str, string *keys);

int main(object caller, object room, string arguments)
{
    string target;

    room = environment(caller);

    if(!stringp(arguments)) return render_room(caller, room) ;
    if(sscanf(arguments, "at %s", target)) return render_object(caller, room, lower_case(target)) ;
    if(sscanf(arguments, "in %s", target)) return render_container(caller, room, lower_case(target)) ;
    else return render_object(caller, room, arguments) ;
}

string highlight_view(string str, string *keys)
{
    int i;

    /* Recode following with regexp? */

    if(sizeof(keys) <= 0) return str;

    for(i = 0; i < sizeof(keys); i++)
    {
        str = replace_string(str , " " + keys[i] + " ", " \e<0065>\e<it1>" + keys[i] + "\e<res> ");
        str = replace_string(str , " " + capitalize(keys[i]) + " ", " \e<0065>\e<it1>" + capitalize(keys[i]) + "\e<res> ");
        str = replace_string(str , " " + keys[i] + ",", " \e<0065>\e<it1>" + keys[i] + "\e<res>,");
        str = replace_string(str , " " + keys[i] + ".", " \e<0065>\e<it1>" + keys[i] + "\e<res>.");
        str = replace_string(str , " " + keys[i] + "!", " \e<0065>\e<it1>" + keys[i] + "\e<res>!");
        str = replace_string(str , " " + keys[i] + ";", " \e<0065>\e<it1>" + keys[i] + "\e<res>;");
        str = replace_string(str , " " + keys[i] + "'", " \e<0065>\e<it1>" + keys[i] + "\e<res>'");
        str = replace_string(str , " " + keys[i] + ":", " \e<0065>\e<it1>" + keys[i] + "\e<res>:");
    }

    return str;
}

int render_room(object caller, object room)
{
    string *exits, *users, *objects;

    if(!objectp(room)) return notify_fail("You see nothing because you have no environment!\n");

    caller->tell("\e<0015>" + room->query_short() + "\e<res>\n\n");
    caller->tell(highlight_view(room->query_long(), keys(room->query_items())) + "\n");

    exits = keys(room->query_exits());
    exits = map(exits, (: "\e<0011>"+$1+"\e<res>" :)) ;

    switch(sizeof(exits))
    {
        case 0 :
        {
            caller->tell("\e<0006>There are no obvious exits here.\e<res>\n");
            break;
        }
        case 1 :
        {
            caller->tell("\e<0006>The only obvious exit is\e<res> "+ exits[0]+"\e<0006>.\e<res>\n");
            break;
        }

        default :
        {
            
            caller->tell("\e<0006>" + sprintf("The only obvious exits are %s\e<0006>, and\e<res> %s\e<0006>.\e<res>\n\n", implode(exits[0..<2], ", "), exits[<1] + NOR)) ;
        }
    }

    users = filter(all_inventory(room), (: living($1) && $1 != $2 :), caller);
    objects = filter(all_inventory(room), (: !living($1) :));
    
    if(sizeof(users) > 0) caller->tell(implode(users->query_short(), "\n") + "\n"); 
    if(sizeof(objects) > 0) caller->tell(implode(objects->query_short(), "\n") + "\n");
    return 1 ;
}

int render_object(object caller, object room, string target)
{
    object ob, user;
    string subtarget;

    if(stringp(room->query_item(target)))
    {
        caller->tell(highlight_view(room->query_item(target), keys(room->query_items())) + "\n");
        return 1;
    }
    
    if(sscanf(target, "%s in %s", target, subtarget) == 2 || 
        sscanf(target, "%s on %s", target, subtarget) == 2 && subtarget != "me")
    {
        user = present(subtarget, room);
        if(!objectp(user)) return notify_fail("You can not find '" + subtarget+ "'.\n");
        ob = present(target, user);
    }
    else
    {
        ob = present(target, room);
        if(!objectp(ob)) ob = present(target, caller);
    }
    
    if(!objectp(ob)) return notify_fail("You can't find '" + target + "'.\n");

    if(living(ob))
    {
        object *inv = all_inventory(ob);
        
        if(ob == caller)
        {
            caller->tell("You look at yourself.\n");
            room->tell(caller->query_cap_name() + " looks at themselfs.\n", ({ ob }));
            
            caller->tell("\t\e<0015>" + ob->query_cap_name() + "\e<res>\n\n");
            caller->tell(ob->query_long() + "\n");
            caller->tell(implode(filter(inv->query("extra_look"), (: $1 :)),"\n") + "\n\n");
            
            if(sizeof(inv) <= 0) caller->tell("You are carrying nothing.\n\n");
            else caller->tell("You are carrying:\n" + implode(inv->query_short(), "\n") + "\n\n");
        }
        else
        {
            caller->tell("You look at " + ob->query_cap_name() + ".\n");
            room->tell(caller->query_cap_name() + " looks at " +ob->query_cap_name() + ".\n", ({ caller, ob }) );
            ob->tell(caller->query_cap_name() + " looks at you.\n");

            caller->tell("\t\e<00015>" + ob->query_cap_name() + "\e<res>\n\n");
            caller->tell(ob->query_long() + "\n");
            caller->tell(implode(filter(inv->query("extra_look"), (: $1 :)),"\n") + "\n\n");

            if(sizeof(inv) <= 0) caller->tell(ob->query_cap_name() + " is carrying nothing.\n\n");
            else caller->tell(ob->query_cap_name() + " is carrying:\n" + implode(inv->query_short(), "\n") + "\n\n");
        }
    }

    else
    {
        if(objectp(user))
        {
            caller->tell("You look at a " + ob->query_short() + " on "+ user->query_cap_name() + "\n\n");
            user->tell(caller->query_cap_name() + " looks you over.\n");
            room->tell(caller->query_cap_name() +" looks over " + ob->query_short() + "\n", ({ caller, user }));
        }
        else
        {
            caller->tell("You look at a " + ob->query_short() + "\n\n");
            room->tell(caller->query_cap_name() + " looks at "+ ob->query_short() + "\n", ({ caller }) );
        }

        caller->tell(ob->query_long() + "\n");
    }

    return 1;
}

int render_container(object caller, object room, string target)
{
    return notify_fail("Error: Containers are not implemented yet.\n");
}

string help()
{
    return(HIW + " SYNTAX:" + NOR + " look [<in/at>] <object> [<on/in> <object>]\n\n" +
      "This command will allow you to look at objects in your environment.\n" +
      "If no command is supplied, it will show you the whole room. You can\n" +
      "also look at specific objects by typing 'look <object>' or 'look at object'.\n" +
      "You might also try looking into an object by type 'look in <object>'.\n");
}
