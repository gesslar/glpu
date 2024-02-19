/* clone.c

 Tacitus @ LPUniversity
 31-OCT-05
 Standard object minipulation command

*/

//Last edited on July 19th, 2006 by Parthenon

inherit STD_CMD ;

mixed main(object caller, object room, string str) {
    object ob;
    string err, custom, tmp;

    if(!str) str = caller->query("cwf");
    if(!str) return "SYNTAX: clone <filename>" ;

    str = append(str, ".c");
    str = resolve_path(caller->query("cwd"), str);

    if(!file_exists(str)) return "Error [clone]: Unable to find file '" + str + "'.";

    err = catch(ob = clone_object(str));

    if(stringp(err) || !ob)
        return "Error [clone]: An error was encountered when cloning the object:\n" + err  ;

    if(ob->move(caller) != MOVE_OK) {
        if(ob->move(environment(caller) != MOVE_OK)) {
            ob->remove() ;
            return "Error [clone]: Unable to move object to your location.";
        }
    }

    if(caller->query_env("custom_clone") && wizardp(caller))
    custom = caller->query_env("custom_clone");

    if(custom) {
        tmp = custom;
        tmp = replace_string(tmp, "$O", (get_short(ob)[0] == 'a' ? get_short(ob) : "a " + get_short(ob)));
        tmp = replace_string(tmp, "$N", caller->query_cap_name());
        tell_room(environment(caller), capitalize(tmp) + "\n", caller);
        write("Success [clone]: New object '" + file_name(ob) + "' cloned.\n");
    } else {
        write("Success [clone]: New object '" + file_name(ob) + "' cloned.\n");
        tell_room(environment(caller),
            capitalize(caller->name()) + " creates a '" + get_short(ob) + "'.\n",
        ({caller}));
    }

    caller->set("cwf", str);
    return 1;
}

string help(object caller) {
    return (" SYNTAX: clone <file>\n\n" +
        "This command produces a clone of a file.\n\n" +
        "See also: dest");
}
