// /std/modules/gmcp.c
// Module inherited by living objects to handle GMCP
//
// Created:     2024/02/22: Gesslar
// Last Change: 2024/02/22: Gesslar
//
// 2024/02/22: Gesslar - Created

#include <daemons.h>
#include <classes.h>

inherit CLASS_GMCP ;

// Functions from other objects
mixed query_env(string var_name);
int set_env(string var_name, string var_value) ;

// Variables
private nosave mapping gmcp_data = ([]) ;

public int gmcp_enabled() {
    return has_gmcp(this_object()) && query_env("gmcp") == "on";
}

void gmcp(string message) {
    class ClassGMCP g ;
    mixed err ;
    string module ;

    g = GMCP_D->convert_message(message) ;

    if(g == null || (g.package == null && g.command == null))
        return ;

    module = DIR_STD_MODULES "gmcp/" + g.package + ".c" ;
    if(!file_exists(module))
        return ;

    if(err = catch(load_object(module)))
        return ;

    if(g.subpackage)
        if(g.payload)
            catch(call_other(module, g.subpackage, g.command, g.payload)) ;
        else
            catch(call_other(module, g.subpackage, g.command)) ;
    else
        if(g.payload)
            catch(call_other(module, g.command, g.payload)) ;
        else
            catch(call_other(module, g.command)) ;
}

varargs void do_gmcp(string package, mixed data) {
    string message ;

    if(data)
        data = json_encode(data) ;
    else
        data = "" ;

    message = trim(sprintf("%s %s", package, data)) ;

    send_gmcp(message) ;
}

mapping query_gmcp_client() {
    return copy(gmcp_data["client"]);
}

mapping query_gmcp_supports() {
    return copy(gmcp_data["supports"]);
}

void set_gmcp_supports(mapping supports) {
    gmcp_data["supports"] = supports ;
}

void set_gmcp_client(mapping client) {
    gmcp_data["client"] = client ;
}
