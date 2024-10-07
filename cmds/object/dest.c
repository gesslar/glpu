/* dest.c

 Tacitus @ LPUniversity
 31-OCT-05
 Standard object minipulation command

*/

//Last edited on July 19th, 2006 by Parthenon
// Last Change: 2024/02/04: Gesslar
// - General formatting

inherit STD_CMD ;

mixed main(object tp, string str) {
    string custom, tmp ;
    object ob, env, room ;
    int cloned ;
    string short, name ;

    name = tp->query_name() ;

    if(!str)
        str = tp->query_env("cwf") ;

    ob = get_object(str) ;
    if(ob && userp(ob) && !adminp(tp))
        return _error("You can't destruct players.") ;

    room = environment(tp) ;

    if(ob) {
        cloned = clonep(ob) ;
        env = environment(ob) ;

        if(living(ob))
            short = ob->query_name() ;
        else
            if(environment(ob))
                short = add_article(get_short(ob)) ;
            else
                short = file_name(ob) ;

        if(tp->query_env("custom_dest") && wizardp(tp))
            custom = tp->query_env("custom_dest") ;

        if(custom) {
            tmp = custom ;
            tmp = replace_string(tmp, "$O", short) ;
            tmp = replace_string(tmp, "$N", name) ;
        }

        catch(ob->remove()) ;
        if(ob) destruct(ob) ;

        if(ob) return _error("Ok that didn't work.") ;
        else {
            if(custom) {
                if(env == room)
                    tell_room(environment(tp), capitalize(tmp) + "\n", tp) ;
                return _ok("Destructed %s.", short) ;
            } else {
                if(env == room)
                    tell_room(environment(tp), name + " destructs " + short + ".\n", tp) ;
                return _ok("Destructed %s.", short) ;
            }
        }
    }

    if(str[<2..<1] != ".c")
        str += ".c" ;

    str = resolve_path(tp->query_env("cwd"), str) ;

    if(ob = find_object(str)) {
        tp->set_env("cwf", str) ;
        ob->remove() ;
        if(ob) destruct(ob) ;
        return _ok("Destructing master object for '%s'.", str) ;
    }

    return _error("Object '%s' not found.", str) ;
}

string help(object tp) {
    return
"SYNTAX: dest <object/filename>\n\n"+
"This command destructs an object from memory.\n\n" +
"See also: clone" ;
}
