//colour.c

//Tacitus @ LPUniversity
//24-OCT-05
//Standard command

mixed main(object caller, string str) {
    string cmd, arg ;

    if(stringp(str)) {
        if(sscanf(str, "%s %s", cmd, arg) != 2) {
            cmd = str ;
        }
    } else {
        cmd = str ;
    }


    switch(cmd) {
        case "enable" :
        case "on" :
        case "start" :
        case "ansi" :
        case "begin" :
            str = "on" ;
            caller->set_env("colour", str);
            return _ok(caller, "Colour " + str + ".");
        case "disable" :
        case "off" :
        case "stop" :
        case "none" :
        case "kill" :
            str = "off" ;
            caller->set_env("colour", str);
            return _ok(caller, "Colour " + str + ".");
        case "list" :
            return _info(caller, "Colour List:\n\n" + COLOUR_D->get_colour_list());
        case "show" : {
            int num ;
            string code ;
            string fg, bg ;

            if(!arg)
                return _error(caller, "Invalid colour code.") ;

            if(caller->query_env("colour") != "on")
                return _error(caller, "Colour is currently disabled.") ;

            if(sscanf(arg, "%d", num) != 1)
                return _error(caller, "Invalid colour code.") ;

            if(num < 0 || num > 255) {
                _error(caller, "Invalid colour code.") ;
                return 1;
            }

            fg = "\e"+sprintf("0%'0'3d", num)+"\e" ;

            return _info(caller, "\n"
                "%s\'%'0'3d\' will appear like this in the foreground.\eres\e\n"
                "\ere1\e%s\'%'0'3d\' will appear like this in the background.\eres\e",
                fg, num, fg, num) ;
        }
        default :
            if(caller->query_env("colour") == "on")
                return _info(caller, "Colour is currently enabled.") ;
            else
                return _info(caller, "Colour is currently disabled.") ;
    }
}

string query_help(object caller) {
    return
"SYNTAX: colour [<enable>|<disable>|list|show [#]]\n\n"
"With no arguments, this command will tell you if you currently have "
"colour enabled or disabled. You can also toggle colour by providing the "
"arguments \eul1\eenable\eul0\e to enable or \eul1\edisable\eul0\e to "
"disable.\n"
"\n"
"Use the \eul1\elist\eul0\e argument to list all the available colours.\n"
"\n"
"Use the \eul1\eshow\eul0\e argument to see how a specific colour code "
"will appear in the foreground and background. The argument must be a "
"number between 0 and 255.\n"
"\n"
"See also: set_env\n" ;
}
