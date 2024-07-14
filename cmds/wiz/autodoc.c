/**
 * @file /cmds/wiz/autodoc.c
 * @description Command to interface with the autodoc system.
 *
 * @created 2024/07/14 - Gesslar
 * @last_modified 2024/07/14 - Gesslar
 *
 * @history
 * 2024/07/14 - Gesslar - Created
 */

#include <daemons.h>

inherit STD_CMD;

mixed main(object tp, string str) {
    string cmd, arg ;
    mixed result ;

    if(!str)
        return query_help(tp);

    if(sscanf(str, "%s %s", cmd, arg) != 2)
        cmd = str;

    switch(cmd) {
        case "scan":
            result = AUTODOC_D->autodoc_scan();
            if(stringp(result))
                return _error(result);
            else
                return _info("Autodoc scan started.");
        default:
            return _error("Invalid command to autodoc.");
    }
}

string query_help(object tp) {
    return
"Syntax: autodoc <command> [args]\n\n"
"Commands:\n"
"    scan - Starts the autodoc scan process.\n"
;
}
