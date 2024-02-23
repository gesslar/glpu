// /adm/daemons/gmcp.c
// GMCP Daemon
//
// Created:     2024/02/22: Gesslar
// Last Change: 2024/02/22: Gesslar
//
// 2024/02/22: Gesslar - Created

#include <classes.h>

inherit STD_DAEMON ;
inherit CLASS_GMCP ;

// Functions
string normalize_package_info(string package_info) ;
class ClassGMCP convert_message(string message) ;

void setup() {
    set_no_clean() ;
}

varargs void send_gmcp(object body, string gmcp_package, mixed arg) {
    mixed *packet ;
    class ClassGMCP gmcp ;
    string module ;
    mixed err ;

    if(!body || !gmcp_package)
        return ;

    gmcp = convert_message(gmcp_package) ;
    if(gmcp == null || (gmcp.package == null && gmcp.command == null))
        return ;

    gmcp.payload = arg ;

    module = __DIR__ "gmcp_modules/" + gmcp.package + ".c" ;

    if(!file_exists(module))
        return ;

    if(err = catch(load_object(module)))
        return ;

    if(gmcp.subpackage)
        if(gmcp.payload)
            catch(call_other(module, gmcp.subpackage, body, gmcp.command, gmcp.payload)) ;
        else
            catch(call_other(module, gmcp.subpackage, body, gmcp.command)) ;
    else
        if(gmcp.payload)
            catch(call_other(module, gmcp.command, body, gmcp.payload)) ;
        else
            catch(call_other(module, gmcp.command, body)) ;
}

class ClassGMCP convert_message(string message) {
    class ClassGMCP gmcp = new(class ClassGMCP) ;
    string *parts ;
    string package_info, message_info ;
    int pos ;
    int sz ;

    message = trim(message) ;
    pos = strsrch(message, " ") ;
    if(pos == -1) {
        package_info = message ;
        message_info = null ;
    } else {
        package_info = message[0..pos-1] ;
        message_info = message[pos+1..] ;
    }

    parts = explode(package_info, ".") ;
    sz = sizeof(parts) ;
    if(sz >= 1) {
        gmcp.package = parts[0] ;
        if(sz >= 2) {
            if(sz == 2)
                gmcp.command = parts[1] ;
            else if(sz == 3) {
                gmcp.subpackage = parts[1] ;
                gmcp.command = parts[2] ;
            }
        }
    }

    if(!gmcp.package)
        return null ;
    if(!gmcp.command && !gmcp.subpackage)
        return null ;

    if(message_info == null) {
        gmcp.payload = null ;
    } else {
        mixed err ;
        err = catch {
            gmcp.payload = json_decode(message_info) ;
        } ;
        if(err) {
            debugf("Error decoding JSON: %O", chop(err, "\n", -1)) ;
        }
    }

    return gmcp ;
}
