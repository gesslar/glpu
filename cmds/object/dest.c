/* dest.c

 Tacitus @ LPUniversity
 31-OCT-05
 Standard object minipulation command

*/

//Last edited on July 19th, 2006 by Parthenon
// Last Change: 2024/02/04: Gesslar
// - General formatting

inherit STD_CMD ;

mixed main(object caller, string str) {
    string custom, tmp;
    object ob;

    if(!str) str = caller->query("cwf");

    if(caller->query_env("custom_dest") && wizardp(caller))
        custom = caller->query_env("custom_dest");

    if(present(str, environment(caller))) {
        ob = present(str, environment(caller));
        if(custom) {
            tmp = custom;
            tmp = replace_string(tmp, "$O", (get_short(ob)[0] == 'a' ? get_short(ob) : "a " + get_short(ob)));
            tmp = replace_string(tmp, "$N", caller->query_name());

            catch(tell_room(environment(caller), capitalize(tmp) + "\n", caller));
            write("Success [dest]: Destroyed object '" + get_short(ob) + "'.\n");
        } else {
            write("Success [dest]: Destroyed object '" + get_short(ob) + "'.\n");
            catch(tell_room(environment(caller), capitalize(caller->query_name()) + " nullifies '" + get_short(ob) + ".\n", ({caller})));
        }
        catch(ob->remove());
        if(ob) destruct(ob);
        return 1;
    }

    if(present(str, caller)) {
        ob = present(str, caller);
        if(custom) {
            tmp = custom;
            tmp = replace_string(tmp, "$O", (get_short(ob)[0] == 'a' ? get_short(ob) : "a " + get_short(ob)));
            tmp = replace_string(tmp, "$N", caller->query_name());

            catch(tell_room(environment(caller), capitalize(tmp) + "\n", caller));
            write("Success [dest]: Destroyed object '" + get_short(ob) + "'.\n");
        } else {
            write("Success [dest]: Destroyed object '" + get_short(ob) + "'.\n");
            catch(tell_room(environment(caller), capitalize(caller->query_name()) + " nullifies '" + get_short(ob) + ".\n", ({caller})));
        }
        ob->remove();
        if(ob) destruct(ob);
        return 1;
    }

    if(find_player(str) && adminp(caller)) {
        ob = find_player(str);
        if(custom) {
            tmp = custom;
            tmp = replace_string(tmp, "$O", (get_short(ob)[0] == 'a' ? get_short(ob) : "a " + get_short(ob)));
            tmp = replace_string(tmp, "$N", caller->query_name());

            catch(tell_room(environment(caller), capitalize(tmp) + "\n", caller));
            write("Success [dest]: Destroyed object '" + get_short(ob) + "'.\n");
        } else {
            write("Success [dest]: Destroyed object '" + get_short(ob) + "'.\n");
            catch(tell_room(environment(caller), capitalize(caller->query_name()) + " nullifies '" + get_short(ob) + ".\n", ({caller})));
        }
        tell_object(ob, "Notice [dest]: You have been nullified and hence disconnected from the mud.\n");
        if(ob) destruct(ob);
        return 1;
    }

    if(str[<2..<1] != ".c") str += ".c";
    str = resolve_path(caller->query("cwd"), str);
    if(ob = find_object(str)) {
        write("Success [dest]: Destructing master object for '" + str + "'.\n");
        caller->set("cwf", str);
        ob->remove() ;
        if(ob) destruct(ob);
        return 1;
    }

    return(notify_fail("Error [dest]: Object '" + str + "' not found.\n"));
}

string help(object caller) {
    return
"SYNTAX: dest <object/filename>\n\n"+
"This command destructs an object from memory.\n\n" +
"See also: clone";
}
