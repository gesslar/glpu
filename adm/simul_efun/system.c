#include "/adm/obj/simul_efun.h"

//system.c

//Tacitus @ LPUniversity
//08-APR-05
//Simul-efuns

#include <config.h>

/* Please keep the following as is */
/**
 * @simul_efun baselib_name
 * @description Returns the name of the base library.
 * @returns {string} - The base library name.
 */
string baselib_name() {
    return "LPUniversity";
}

/**
 * @simul_efun baselib_version
 * @description Returns the version of the base library.
 * @returns {string} - The base library version.
 */
string baselib_version() {
    return "A-0610-R7" ;
}
/* Thanks */

/**
 * @simul_efun mud_name
 * @description Returns the name of the MUD.
 * @returns {string} - The name of the MUD.
 */
string mud_name() {
    return MUD_NAME ;
}

/**
 * @simul_efun lib_name
 * @description Returns the name of the library being used by the MUD.
 * @returns {string} - The library name.
 */
string lib_name() {
    return mud_config("LIB_NAME") ;
}

/**
 * @simul_efun lib_version
 * @description Returns the version of the library being used by the MUD.
 * @returns {string} - The library version.
 */
string lib_version() {
    return mud_config("LIB_VERSION") ;
}

/**
 * @simul_efun open_status
 * @description Returns the open status of the MUD.
 * @returns {string} - The open status of the MUD.
 */
string open_status() {
    return mud_config("OPEN_STATUS") ;
}

/**
 * @simul_efun admin_email
 * @description Returns the admin email address for the MUD.
 * @returns {string} - The admin email address.
 */
string admin_email() {
    return mud_config("ADMIN_EMAIL") ;
}

/**
 * @simul_efun port
 * @description Returns the port number the MUD is running on.
 * @returns {int} - The port number.
 */
int port() {
    return __PORT__;
}

/**
 * @simul_efun driver_version
 * @description Returns the version of the MUD driver.
 * @returns {string} - The driver version.
 */
string driver_version() {
    string version = __VERSION__ ;
    string name, rest ;

    sscanf(version, "%s %s", name, rest) ;
    if(strsrch(rest, "uncommited") != -1)
        sscanf(rest, "%s-uncommited", rest) ;

    return sprintf("%s %s", name, rest) ;
}

/**
 * @simul_efun arch
 * @description Returns the architecture of the system the MUD is running on.
 * @returns {string} - The system architecture.
 */
string arch() {
    return __ARCH__;
}

/**
 * @simul_efun mud_config
 * @description Retrieves a specific configuration value from the MUD config.
 * @param {string} str - The configuration key to retrieve.
 * @returns {mixed} - The configuration value.
 */
mixed mud_config(string str) {
    return (mixed)CONFIG_D->get_mud_config(str) ;
}

/**
 * @simul_efun log_dir
 * @description Returns the directory where log files are stored.
 * @returns {string} - The log directory.
 */
string log_dir() {
    return mud_config("LOG_DIR") ;
}

/**
 * @simul_efun tmp_dir
 * @description Returns the directory where temporary files are stored.
 * @returns {string} - The temporary directory.
 */
string tmp_dir() {
    return mud_config("TMP_DIR") ;
}

/**
 * @simul_efun debug
 * @description Logs a debug message, optionally formatted with arguments.
 * @param {string} str - The debug message.
 * @param {mixed} [args] - Optional arguments to format the message.
 */
varargs void _debug(mixed str, mixed args...) {
    if(stringp(str)) {
        if(sizeof(args))
            str = sprintf(str, args...) ;
    } else {
        str = sprintf("%O", str) ;
    }

    str = COLOUR_D->substitute_colour(str, "xterm") ;

    debug_message(str) ;
}

/**
 * @function _format_message
 * @description Formats a message with a color and the name of the previous object.
 * @param {string} color - The color to use for the message.
 * @param {string} str - The message to format.
 * @param {mixed} [args] - Optional arguments to format the message.
 * @returns {string} - The formatted message or null if there is no previous object.
 */
private string _format_message(string color, string str, mixed args...) {
    object po ;
    string name ;

    if(!po = previous_object())
        return null ;

    name = query_file_name(po) ;
    name = upper_case(name) ;
    name = replace_string(name, "_", " ") ;
    name = "["+color+name+"\e<res>] " ;

    str = name + str ;
    str = sprintf(str, args...) ;

    return str ;
}

/**
 * @simul_efun _ok
 * @description Provides an informational message, optionally formatted with arguments.
 * @param {string} str - The informational message.
 * @param {mixed} [args] - Optional arguments to format the message.
 * @returns {int} - Always returns 1, unless there is no previous object.
 */
int _ok(mixed str, mixed args...) {
    object tp ;
    string mess ;

    mess = _format_message("\e<0036>", str, args...) ;
    if(nullp(mess))
        return 0 ;

    if(tp = this_player())
        tell(tp, mess + "\n") ;
    else
        _debug(mess) ;

    return 1 ;
}

/**
 * @simul_efun _error
 * @description Provides an error message, optionally formatted with arguments.
 * @param {string} str - The error message.
 * @param {mixed} [args] - Optional arguments to format the message.
 * @returns {int} - Always returns 1, unless there is no previous object.
 */
int _error(mixed str, mixed args...) {
    object tp ;
    string mess ;

    mess = _format_message("\e<0124>", str, args...) ;
    if(nullp(mess))
        return 0 ;

    if(tp = this_player())
        tell(tp, mess + "\n") ;
    else
        _debug(mess) ;

    return 1 ;
}

/**
 * @simul_efun _warn
 * @description Provides a warning message, optionally formatted with arguments.
 * @param {string} str - The warning message.
 * @param {mixed} [args] - Optional arguments to format the message.
 * @returns {int} - Always returns 1, unless there is no previous object.
 */
int _warn(mixed str, mixed args...) {
    object tp ;
    string mess ;

    mess = _format_message("\e<0214>", str, args...) ;
    if(nullp(mess))
        return 0 ;

    if(tp = this_player())
        tell(tp, mess + "\n") ;
    else
        _debug(mess) ;

    return 1 ;
}
