/* beep.c

 Tacitus @ LPUniversity
 14-JULY-06
 Beep command

*/

inherit STD_CMD ;

int main(object caller, object room, string argument)
{
    object user;

    if(!argument) return notify_fail("Error [beep]: Syntax: beep <user>\n");

    user = find_player(argument);
    if(!objectp(user)) return notify_fail("Error [beep]: User '" + argument
      + "' not found.\n");
    if(user->query_env("disable_beep") == "true")
    return notify_fail("Error [beep]: That user has disabled beeps.\n");

    if(user == this_player()) return notify_fail("You beep yourself.\n\a");

    tell_object(user, capitalize(this_player()->query_name())
        + " has beepedyou.\a\n");
    write("You beep " + capitalize(user->query_name()) + ".\n");

    return 1;
}

string help(object caller)
{
    return(" SYNTAX: beep <user>\n\n"
      "This command will 'beep' another user by sending the other user a message that\n"
      "has the bell escape sequence in it. If their client supports it, they'll get a\n"
      "system beep (or some other notification) on their end to alert them to the\n"
      "window. This is useful for getting someone's attention when they might be just\n"
      "busy in another window.\n");
}
