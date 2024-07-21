/* where.c

 JohnDoe @ LPUni
 October 31st, 2005
 Wizard command

QC Review: Tacitus on October 31st, 2005

Last edited on October 31st, 2005 by Tacitus

*/

inherit STD_CMD ;

mixed main(object caller, string str) {
    object ob;

    if(!str) str = this_body()->query_proper_name();

    if(find_player(str)) {
        ob = find_player(str);
        if(environment(ob))
            write(capitalize(ob->query_proper_name()) + " is in " +
                file_name(environment(ob)) + " (" + environment(ob)->query_short() + ").\n");
        else
            write(capitalize(ob->query_proper_name()) + " does not currently have an environment.\n");
        return 1;
    }

    return(notify_fail("Error [where]: Player '" + capitalize(str) + "' not found.\n"));
}

string help(object caller) {
    return(" SYNTAX: where <player>\n\n"
        "This command will tell you where a user is currently located.\n"
        "If you do not provide an argument, it will default to yourself.\n");
}
