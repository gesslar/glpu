/* beep.c

 Tacitus @ LPUniversity
 14-JULY-06
 Beep command

*/

// Last Change: 2024/02/04: Gesslar
// - General formatting and code updates to latest supported standards.

inherit STD_CMD ;

mixed main(object caller, object room, string argument) {
    object user;

    if(!argument)
        return "Error [beep]: Syntax: beep <user>" ;

    user = find_player(argument);
    if(!objectp(user))
        return "Error [beep]: User '" + argument + "' not found.";

    if(user->query_env("disable_beep") == "true")
        return "Error [beep]: That user has disabled beeps.";

    if(user == caller)
        return "You beep yourself.\a";

    tell(user, caller->query_cap_name() + " has beeped you.\a\n");
    tell(caller, "You beep " + user->query_cap_name() + ".\n");

    return 1;
}

string help(object caller) {
    return
"SYNTAX: beep <user>\n\n"
"This command will 'beep' another user by sending the other user a message "
"that has the bell escape sequence in it. If their client supports it, they'll "
"get a system beep (or some other notification) on their end to alert them to "
"the window. This is useful for getting someone's attention when they might be "
"just busy in another window.";
}
