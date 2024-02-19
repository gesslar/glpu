/* what.c

 Tacitus @ LPUniversity
 12-AUG-06
 Displays acitivity info about users

*/

inherit STD_CMD ;

int main(object tp, object room, string arg) {
    if(!adminp(tp)) return 0;
    tell(tp, "=----------------------------------------------=\n");
    tell(tp, sprintf(" %-15s %-s \n", "Username", "Last Command")) ;
    tell(tp, "=----------------------------------------------=\n");

    foreach(object user in users()) {
        string *command_history = user->query_command_history();
        if(!living(user)) continue;

        if(sizeof(command_history) <= 0)
            tell(tp, sprintf("  %-15s %s\n", user->query_cap_name(), "<none>"));
        else
            tell(tp, sprintf("  %-15s %s\n", user->query_cap_name(), command_history[sizeof(command_history)-1]));
    }

    tell(tp, "=----------------------------------------------=\n");

    return 1;
}

string help(object caller)
{
    return(" SYNTAX: what\n\n"
      "This command allows you to view the last command executed by\n"
      "all users logged in. It is recommended that you review any\n"
      "privacy policy that your mud might have before using this tool.\n");
}
