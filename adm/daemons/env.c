/**
 * @file /adm/daemons/env.c
 * @description Fetches environment variables from the system using
 *              external_start(). You will need PACKAGE_EXTERNAL compiled into
 *              the driver and your config file needs to have the correct
 *              path to /adm/bash/env, and it must be executable.
 *
 *              All variables are returned as string.
 *
 *              If you wish to cache the environment variable that you fetch,
 *              you can pass 3 to the second parameter. Cache will not persist
 *              if this object is unloaded.
 *
 * @created 2024/07/06 - Gesslar
 * @last_modified 2024/07/06 - Gesslar
 *
 * @history
 * 2024/07/06 - Gesslar - Created
 */

#include <external.h>

inherit STD_DAEMON ;
inherit M_LOG ;

// Callbacks
private nomask void read_callback(int fd, string str) ;
private nomask void write_callback(int fd) ;
private nomask void close_callback(int fd) ;

private nomask mapping env_cache = ([ ]);
private nomask mapping calls = ([ ]) ;
private nomask string env_file ;

void setup() {
    set_no_clean(1) ;
    env_cache = ([ ]) ;
    env_file = mud_config("ENV_PATH") ;
}

/**
 * Fetches an environment variable from the system.
 *
 * @param string var The name of the environment variable to fetch.
 * @param mixed callback A callback to call when the environment variable is
 *                       fetched. Called with the value of the environment
 *                       variable.
 * @param def The default value to return if the environment variable is not set.
 * @param int cache Whether or not to cache the result.
 * @return void
 */

varargs void fetch(string var, mixed callback, string def, int cache) {
    string result ;
    int fd ;
    mixed err ;

    if(!stringp(var))
        error("Invalid argument 1 to fetch()") ;

    if(!stringp(callback) && !valid_function(callback))
        error("Invalid argument 2 to fetch()") ;

    if (cache && env_cache[var]) {
        if(valid_function(callback)) {
            (*callback)(env_cache[var]) ;
        } else if(stringp(callback)) {
            if(!objectp(previous_object()))
                return ;

            call_other(previous_object(), callback, env_cache[var]) ;
        }
        return ;
    }

    err = catch(fd = external_start(
        EXTERNAL_ENV,
        ({ env_file, var }),
        (: read_callback :),
        (: write_callback :),
        (: close_callback :)
    )) ;

    calls[fd] = ([
        "var" : var,
        "callback" : callback,
        "default" : def,
        "cache" : cache,
        "caller" : previous_object(),
    ]) ;
}

void read_callback(int fd, string str) {
    string var, value, def ;
    mixed callback ;
    int cache ;
    mapping call ;

    if(!call = calls[fd])
        return ;

    map_delete(calls, fd) ;

    callback = call["callback"] ;
    def = call["default"] ;
    cache = call["cache"] ;

    if(sscanf(str, "%s=%s", var, value) != 2) {
        value = def ;
    } else {
        if(var != call["var"]) {
            value = def ;
        } else {
            value = chop(value, "\n", -1) ;
        }
    }

    if(call["cache"]) {
        env_cache[var] = value ;
    }

    if(valid_function(callback)) {
        (*callback)(value) ;
    } else if(stringp(call["callback"])) {
        if(!objectp(call["caller"]))
            return ;

        call_other(call["caller"], call["callback"], value) ;
    }
}

void write_callback(int fd) {
}

void close_callback(int fd) {
    map_delete(calls, fd) ;
}
