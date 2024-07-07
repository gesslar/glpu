#include "/adm/obj/simul_efun.h"

//system.c

//Tacitus @ LPUniversity
//08-APR-05
//Simul-efuns

#include <config.h>

/* Please keep the following as is */
string baselib_name() {
    return "LPUniversity";
}

string baselib_version() {
    return "A-0610-R7" ;
}
/* Thanks */

string mud_name() {
    return MUD_NAME ;
}

string lib_name() {
    return mud_config("LIB_NAME") ;
}

string lib_version() {
    return mud_config("LIB_VERSION") ;
}

string open_status() {
    return mud_config("OPEN_STATUS") ;
}

string admin_email() {
    return mud_config("ADMIN_EMAIL") ;
}

int port() {
    return __PORT__;
}

string driver_version() {
    string version = __VERSION__ ;
    string name, rest ;

    sscanf(version, "%s %s", name, rest) ;
    if(strsrch(rest, "uncommited") != -1)
        sscanf(rest, "%s-uncommited", rest) ;

    return sprintf("%s %s", name, rest) ;
}

string arch() {
    return __ARCH__;
}

mixed mud_config(string str) {
    return (mixed)CONFIG_D->get_mud_config(str) ;
}

string log_dir() {
    return mud_config("LOG_DIR") ;
}

string tmp_dir() {
    return mud_config("TMP_DIR") ;
}

varargs void debug(string str, mixed args...) {
    if(sizeof(args))
        str = sprintf(str, args...) ;

    str = XTERM256->substitute_colour(str, "xterm") ;

    debug_message(str) ;
}

varargs void debugf(string str, mixed args...) {
    return debug(str, args...) ;
}
