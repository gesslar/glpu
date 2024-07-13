/* tell.c

   Gwegster @ LPUniversity
   30-JUNE-05
   a tell command

   Last edited on October 6th, 2006 by Tricky

*/

inherit STD_CMD ;

void send_tell(object o_user, mixed d_user, string d_mud, string msg);
void send_emoteto(object o_user, mixed d_user, string d_mud, string msg);

mixed main(object caller, string str) {
    string who, message;
    object user, c;

    if(sscanf(str, "%s %s", who, message) != 2)
        return "Usage: tell <player> <message>\n" ;

    if(!message)
        return "Incorrect syntax, please check the help.\n" ;

    user = find_living(lower_case(who));

    if(!objectp(user))
        return notify_fail("Error [tell]: User " + who + " is not found.\n");

    if(user->query_env("away")) {
        write("That user is currently away" + (user->query_env("away") != "" ?
            ": (" + user->query_env("away") + ")\n" : ".\n"));

        return 1;
    }

    if(user == caller) {
        if(message[0] == ':')
            write("You emote to yourself: " + user->query_s() + " " + message[1..<1] + "\n");
        else
            write("You tell yourself: " + message + "\n");

        tell_room(environment(caller),
            capitalize(caller->query_name()) + " starts talking to themself.\n", caller);
        caller->set("retell", query_privs(user));

        return 1;
    }

    if(message[0] == ':') {
        tell_object(user, caller->query_name() + " " +
            message[1..<1] + "\n");
        write("You emote to " + capitalize(who) + ": " +
            caller->query_name() + " " + message[1..<1] + "\n");
    } else {
        tell_object(user, capitalize((string)caller->query_name())
            + " tells you: " + message + "\n");
        write("You tell " + capitalize(who) + ": " + message + "\n");
    }

    user->set("reply", query_privs(caller));
    caller->set("retell", query_privs(user));

    return 1;
}

string help(object caller) {
    return(" SYNTAX: tell <player>[@<mud>: | . | :]<message>\n\n"
      "This command will send a message to the specified player if they\n"
      "are online. For example, if you type 'tell tacitus hey' then\n"
      "he'll see '" + capitalize(caller->query_name()) +
      " tells you: hey'. If you use 'tell . <message>\n" +
      "it will send the message to the last person that you used tell to talk to.\n" +
      "You may also talk to a user on another mud in the I3 network using the\n" +
      "'tell <user>@<mud>: <message>' syntax. You may also emote to another playter\n" +
      "using the tell <user> :<message> syntax.\n\n" +
      "See also: say, channel\n");
}
