/*

    afk.c

    Parthenon@LPUniversity
    August 15th, 2006
    Command to set yourself as away

*/

inherit STD_CMD ;

mixed main(object caller, string args) {
    if(!args) args = "";

    caller->set_env("away", args);

    tell(caller, "You are now away" + (!args || args == "" ? ".\n" : ": " + args + "\n"));
    tell(caller, "Press [ENTER] to return.\n");
    input_to("i_Return", caller) ;
    return 1;
}

void i_Return(string arg, object caller) {
    if(!caller)
        return;

    tell(caller, "You return from away\n");
    caller->set_env("away", 0);
    return;
}

string help(object caller) {
    return
    " SYNTAX: afk [<message>]\n\n" +
    "This command allows you to set an away message. While away\n"+
    "you cannot perform any actions. If you set the optional away\n"+
    "message, then when others try to talk to you using things\n"+
    "such as tell, they will receieve that message.\n";
}
