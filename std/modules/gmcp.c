// /std/modules/gmcp.c
// Module inherited by living objects to handle GMCP
//
// Created:     2024/02/22: Gesslar
// Last Change: 2024/02/22: Gesslar
//
// 2024/02/22: Gesslar - Created

#include <daemons.h>
#include <classes.h>
#include <type.h>

inherit CLASS_GMCP ;

// Functions from other objects
mixed query_env(string var_name);
int set_env(string var_name, string var_value) ;

// Functions
private mixed gmcp_stringify(mixed data) ;

public int gmcp_enabled() {
    return has_gmcp(this_object()) && query_env("gmcp") == "on";
}

void gmcp(string message) {
    class ClassGMCP gmcp ;
    mixed err ;
    string module ;

    gmcp = GMCP_D->convert_message(message) ;

    if(gmcp == null || (gmcp.package == null && gmcp.command == null))
        return ;

    module = DIR_STD_MODULES "gmcp/" + gmcp.package + ".c" ;
    if(!file_exists(module))
        return ;

    if(err = catch(load_object(module)))
        return ;

    if(gmcp.subpackage)
        if(gmcp.payload)
            catch(call_other(module, gmcp.subpackage, gmcp.command, gmcp.payload)) ;
        else
            catch(call_other(module, gmcp.subpackage, gmcp.command)) ;
    else
        if(gmcp.payload)
            catch(call_other(module, gmcp.command, gmcp.payload)) ;
        else
            catch(call_other(module, gmcp.command)) ;
}

varargs void do_gmcp(string package, mixed data) {
    string message ;

    if(data)
        data = json_encode(gmcp_stringify(data)) ;
    else
        data = "" ;

    message = trim(sprintf("%s %s", package, data)) ;

    send_gmcp(message) ;
}

mixed gmcp_stringify(mixed data) {
    switch(typeof(data)) {
        case T_STRING:
            return data ;
        case T_INT:
            return sprintf("%d", data) ;
        case T_ARRAY: {
                mixed *result ;

                result = map(data, (: gmcp_stringify :)) ;

                return result ;
            }
        case T_OBJECT:
            return "" ;
        case T_MAPPING: {
                mapping result = ([ ]) ;
                mixed key, value ;

                foreach(key, value in data) {
                    result[gmcp_stringify(key)] = gmcp_stringify(value) ;
                }

                return result ;
            }
        case T_FUNCTION:
            return "" ;
        case T_FLOAT:
            return sprintf("%f", data) ;
        case T_BUFFER:
            return "" ;
        case T_CLASS:
            return "" ;
        default:
            return "" ;
    }

    return "" ;
}
