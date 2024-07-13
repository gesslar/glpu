//colour.c

//Tacitus @ LPUniversity
//24-OCT-05
//Standard command

mixed main(object caller, string str) {
    switch(str)
    {
    case "enable" :
    case "on" :
    case "start" :
    case "ansi" :
    case "begin" : {
        caller->set_env("colour", "enabled");
        tell(caller, "Success: Colour " + str + ".\n");
        return 1;
    }

    case "disable" :
    case "off" :
    case "stop" :
    case "none" :
    case "kill" : {
        caller->set_env("colour", "disabled");
        tell(caller, "Success: Colour " + str + ".\n");
        return 1;
    }
    case "list" :
        tell(caller, "Colour List:\n\n" + XTERM256->get_colour_list() + "\n");
        return 1 ;
    default : {
        if(caller->query_env("colour") == "enabled") {
            tell(caller, "Colour: Colour is currently enabled.\n") ;
            return 1;
        } else {
            tell(caller, "Colour: Colour is currently disabled.\n") ;
            return 1;
        }
    }
    }
}

string help(object caller)
{
    return(" SYNTAX: colour [<enable>||<disable>]\n\n"
      "With no arguments, this command will tell you if you currently\n"
      "have colour enabled or disabled. You can also toggle colour by\n"
      "providing the arguments 'enable' to enable or 'disable' to disable.\n"
      "It should be noted that 'enable' and 'disable' aren't the only\n"
      "keywords to toggle colour, you can also use 'off', 'start',\n"
      "and 'none' to name a few. How many can you find?\n\n"
      "See also: set_env\n");
}
