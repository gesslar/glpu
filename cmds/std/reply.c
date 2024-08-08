/* reply.c

 Tacitus @ LPUniversity
 05-MAY-06
 Std reply command

*/

//Last edited on August 11th, 2006 by Parthenon
//Last edited on September 10th, 2006 by Tricky
//Last edited on September 18th, 2006 by Tricky

inherit STD_CMD ;

mixed main(object caller, string message) {
    object user;
    string d_user, d_mud;
    string who = caller->query("reply");

    if(!message)
        return(notify_fail("Error [tell]: You must provide an argument. Syntax: reply <message>\n"));

    user = find_player(who);

    if(!objectp(user))
        return notify_fail("Error [tell]: User " + who + " is not found.\n");

    if(user == caller) {
        if(message[0] == ':')
            message = message[1..];

        write("You reply to yourself: " + message + "\n");
        tell_room(environment(caller), caller->query_name() + " starts talking to themselves.\n", caller);

        return 1;
    }

    if(message[0] == ':') {
        tell_object(user, caller->query_name() + " " + message[1..] + "\n");
        write("You reply to " + capitalize(who) + ": " + caller->query_name() + " " + message[1..] + "\n");
    } else {
        tell_object(user, caller->query_name() + " tells you: " + message + "\n");
        write("You reply to " + capitalize(who) + ": " + message + "\n");
    }

    user->set("reply", query_privs(caller));

    return 1;
}

string help(object caller) {
    return(" SYNTAX:" + " reply <message>\n\n"
    "This command will send a message to the last user that sent\n"
    "you a message if they are still online.\n\n"
    "See also: " + "say, channel\n");
}
