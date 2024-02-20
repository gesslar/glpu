/* clone.c

 Tacitus @ LPUniversity
 31-OCT-05
 Standard object minipulation command

*/

//Last edited on July 19th, 2006 by Parthenon

inherit STD_CMD ;

mixed main(object caller, object room, string str) {
    object ob, dest ;
    string err, custom, tmp, short, file ;
    int result ;

    if(!str) str = caller->query("cwf");
    if(!str) return "SYNTAX: clone <filename>" ;

    str = append(str, ".c");
    str = resolve_path(caller->query("cwd"), str);

    if(!file_exists(str))
        return "Error [clone]: Unable to find file '" + str + "'.";

    err = catch(ob = clone_object(str));

    if(stringp(err) || !ob)
        return "Error [clone]: An error was encountered when cloning the object:\n" + err  ;

    short = get_short(ob);
    file = file_name(ob);
    dest = caller ;
    if(!(result = ob->move(dest)) & MOVE_OK) {
        printf("Result: %d\n", result);
        dest = environment(caller);
        if(!(result = ob->move(dest) & MOVE_OK)) {
            printf("Result: %d\n", result);
            ob->remove() ;
            return "Error [clone]: Unable to move object to your location.";
        }
    }

    if(caller->query_env("custom_clone") && wizardp(caller))
    custom = caller->query_env("custom_clone");

    if(custom) {
        tmp = custom;
        tmp = replace_string(tmp, "$O", short);
        tmp = replace_string(tmp, "$N", caller->query_cap_name());
        tell_room(environment(caller), capitalize(tmp) + "\n", caller);
        write("Success [clone]: New object '" + file + "' cloned to " +
            get_short(dest) + " (" +file_name(dest)+ ").\n") ;
    } else {
        write("Success [clone]: New object '" + file + "' cloned to " +
            get_short(dest) + " (" +file_name(dest)+ ").\n") ;
        tell_room(environment(caller),
            capitalize(caller->name()) + " creates " + short + ".\n", caller);
    }

    caller->set("cwf", str);
    return 1;
}

string help(object caller) {
    return (" SYNTAX: clone <file>\n\n" +
        "This command produces a clone of a file.\n\n" +
        "See also: dest");
}
