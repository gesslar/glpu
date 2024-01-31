/* clone.c

 Tacitus @ LPUniversity
 31-OCT-05
 Standard object minipulation command

*/

//Last edited on July 19th, 2006 by Parthenon

inherit CMD ;

int main(object caller, object room, string str)
{
    object ob;
    string err, custom, tmp;

    if(!str) str = caller->query("cwf");

    if(!str) return(notify_fail("SYNTAX: clone <filename>\n"));

    if(str[<2..<1] != ".c") str += ".c";

    str = resolve_path(caller->query("cwd"), str);

    if(!file_exists(str)) return(notify_fail("Error [clone]: Unable to find file '" + str + "'.\n"));

    write("Notice [clone]: Cloning file '" + str + "' to environment...\n");
    err = catch(ob = clone_object(str));

    if(stringp(err) || !ob)
    return(notify_fail("Error [clone]: An error was encountered when cloning the object:\n" + err + "\n"));

    ob->move(environment(caller));

    if(caller->query_env("custom_clone") && wizardp(caller))
    custom = caller->query_env("custom_clone");

    if(custom)
    {
    tmp = custom;
    tmp = replace_string(tmp, "$O", (ob->query_short()[0] == 'a' ? ob->query_short() : "a " + ob->query_short()));
    tmp = replace_string(tmp, "$N", caller->query_cap_name());
    tell_room(environment(caller), capitalize(tmp) + "\n", caller);
    write("Success [clone]: New object '" + file_name(ob) + "' cloned.\n");
    }
    else
    {
    write("Success [clone]: New object '" + file_name(ob) + "' cloned.\n");
    tell_room(environment(caller),
      capitalize(caller->query_name()) + " creates a '" + ob->query_short() + "'.\n",
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
