//colour.c

//Tacitus @ LPUniversity
//24-OCT-05
//Standard command

mixed main(object caller, string str) {
    switch(str) {
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
            tell(caller, "Colour List:\n\n" + COLOUR_D->get_colour_list() + "\n");
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

string query_help(object caller) {
    return
    "SYNTAX: colour [<enable>|<disable>]\n\n"
    "With no arguments, this command will tell you if you currently have "
    "colour enabled or disabled. You can also toggle colour by providing the "
    "arguments \ebl1\eenable\ebl0\e to enable or \ebl1\edisable\ebl0\e to "
    "disable.\n"
    "See also: set_env\n" ;
}
