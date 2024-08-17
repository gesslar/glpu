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
        case "on" :
        case "begin" :
            str = "on" ;
            caller->set_env("colour", str);
            return _ok(caller, "Colour " + str + ".");
        case "off" :
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

            if(caller->query_pref("colour") != "on")
                return _error(caller, "Colour is currently disabled.") ;

            if(sscanf(arg, "%d", num) != 1)
                return _error(caller, "Invalid colour code.") ;
            if(num < 0 || num > 255) {
                _error(caller, "Invalid colour code.") ;
                return 1;
            }

            fg = "{{"+sprintf("0%'0'3d", num)+"}}" ;

            return _info(caller, "\n"
                "%s\'%'0'3d\' will appear like this in the foreground.{{res}}\n"
                "{{re1}}%s\'%'0'3d\' will appear like this in the background.{{res}}",
                fg, num, fg, num) ;
        }
        default :
            if(caller->query_pref("colour") == "on")
                return _info(caller, "Colour is currently enabled.") ;
            else
                return _info(caller, "Colour is currently disabled.") ;
    }
}

string query_help(object caller) {
    return
"SYNTAX: colour [<on>|<off>|list|show [#]]\n\n"
"With no arguments, this command will tell you if you currently have "
"colour enabled or disabled. You can also toggle colour by providing the "
"arguments {{ul1}}on{{ul0}} to enable or {{ul1}}off{{ul0}} to "
"disable.\n"
"\n"
"Use the {{ul1}}list{{ul0}} argument to list all the available colours.\n"
"\n"
"Use the {{ul1}}show{{ul0}} argument to see how a specific colour code "
"will appear in the foreground and background. The argument must be a "
"number between 0 and 255.\n"
"\n"
"See also: set\n" ;
}
