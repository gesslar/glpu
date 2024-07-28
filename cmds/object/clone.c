/* clone.c

 Tacitus @ LPUniversity
 31-OCT-05
 Standard object minipulation command

*/

//Last edited on July 19th, 2006 by Parthenon

inherit STD_CMD ;

mixed main(object caller, string str) {
    object ob, dest ;
    string err, custom, tmp, short, file ;
    int result ;

    if(!str) str = caller->query_env("cwf");
    if(!str) return "SYNTAX: clone <filename>" ;

    str = append(str, ".c");
    str = resolve_path(caller->query_env("cwd"), str);

    // if(!file_exists(str))
    //     return "Error [clone]: Unable to find file '" + str + "'.";

    err = catch(ob = new(str));

    if(stringp(err) || !ob)
        return "Error [clone]: An error was encountered when cloning the object:\n" + err  ;

    short = get_short(ob);
    file = file_name(ob);
    dest = caller ;
/*
#define MOVE_OK             ( 1 << 0)
#define MOVE_TOO_HEAVY      ( 1 << 1)
#define MOVE_NO_ROOM        ( 1 << 2)
#define MOVE_NO_DEST        ( 1 << 3)
#define MOVE_NOT_ALLOWED    ( 1 << 4)
#define MOVE_DESTRUCTED     ( 1 << 5)
*/

    result = ob->move(dest) ;
    if(result & MOVE_OK) {
        if(caller->query_env("custom_clone") && wizardp(caller))
            custom = caller->query_env("custom_clone");
        if(custom) {
            tmp = custom;
            tmp = replace_string(tmp, "$O", short);
            tmp = replace_string(tmp, "$N", caller->query_name());
            tell_room(environment(caller), capitalize(tmp) + "\n", caller);
            write("Success [clone]: New object '" + file + "' cloned to " +
                get_short(dest) + " (" +file_name(dest)+ ").\n") ;
        } else {
            write("Success [clone]: New object '" + file + "' cloned to " +
                get_short(dest) + " (" +file_name(dest)+ ").\n") ;
            tell_room(environment(caller),
                capitalize(caller->query_name()) + " creates " + short + ".\n", caller);
        }
        caller->set_env("cwf", str);
        return 1;
    } else {
        if(result & MOVE_TOO_HEAVY)
            return "Error [clone]: The object is too heavy to carry.";
        if(result & MOVE_NO_ROOM)
            return "Error [clone]: There is not enough room to carry the object.";
        if(result & MOVE_NO_DEST)
            return "Error [clone]: The object has no destination.";
        if(result & MOVE_NOT_ALLOWED)
            return "Error [clone]: You are not allowed to carry the object.";
        if(result & MOVE_DESTRUCTED)
            return "Error [clone]: The object has been destructed.";
    }

    if(caller->query_env("custom_clone") && wizardp(caller))
    custom = caller->query_env("custom_clone");

    if(custom) {
        tmp = custom;
        tmp = replace_string(tmp, "$O", short);
        tmp = replace_string(tmp, "$N", caller->query_name());
        tell_room(environment(caller), capitalize(tmp) + "\n", caller);
        write("Success [clone]: New object '" + file + "' cloned to " +
            get_short(dest) + " (" +file_name(dest)+ ").\n") ;
    } else {
        write("Success [clone]: New object '" + file + "' cloned to " +
            get_short(dest) + " (" +file_name(dest)+ ").\n") ;
        tell_room(environment(caller),
            caller->query_name() + " creates " + short + ".\n", caller);
    }

    caller->set_env("cwf", str);
    return 1;
}

string help(object caller) {
    return (" SYNTAX: clone <file>\n\n" +
        "This command produces a clone of a file.\n\n" +
        "See also: dest");
}
