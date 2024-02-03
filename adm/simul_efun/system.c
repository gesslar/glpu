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
    return __VERSION__;
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
