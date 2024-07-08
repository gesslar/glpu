// /adm/daemons/gmcp.c
// GMCP Daemon
//
// Created:     2024/02/22: Gesslar
// Last Change: 2024/02/22: Gesslar
//
// 2024/02/22: Gesslar - Created

#include <classes.h>
#include <gmcp.h>

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
    string gmcp_module ;
    mixed err ;
    object ob ;

    if(!get_config(__RC_ENABLE_GMCP__))
        return ;

    if(!body || !gmcp_package)
        return ;

    if(base_name(body) == LOGIN_OB) {
        if(!has_gmcp(body))
            return ;
    } else if(base_name(body) == STD_BODY) {
        if(!body->gmcp_enabled())
            return ;
    } else {
        return ;
    }

    gmcp = convert_message(gmcp_package) ;
    if(gmcp == null || (gmcp.package == null && gmcp.module == null))
        return ;

    gmcp.payload = arg ;

    gmcp_module = __DIR__ "modules/gmcp/" + gmcp.package + ".c" ;

    if(!file_exists(gmcp_module)) {
        log_file("system/gmcp", "[%s] Module %s not found [%O]",
            ctime(),
            gmcp_module,
            previous_object()->query_gmcp_module() ?
                previous_object(1) :
                previous_object()
        ) ;
        return ;
    }

    if(err = catch(ob = load_object(gmcp_module)))
        return ;

    if(!function_exists(gmcp.module, ob)) {
        log_file("system/gmcp", "[%s] [%s] Function %s not found in %s [%O]",
            ctime(),
            __DIR__ "modules/gmcp",
            gmcp.module,
            gmcp.package,
            previous_object()
        ) ;
        return ;
    }

    if(gmcp.submodule)
        if(gmcp.payload)
            catch(call_other(gmcp_module, gmcp.module, body, gmcp.submodule, gmcp.payload)) ;
        else
            catch(call_other(gmcp_module, gmcp.module, body, gmcp.submodule)) ;
    else
        if(gmcp.payload)
            catch(call_other(gmcp_module, gmcp.module, body, gmcp.payload)) ;
        else
            catch(call_other(gmcp_module, gmcp.module, body)) ;
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

    gmcp.name = package_info ;

    parts = explode(package_info, ".") ;
    sz = sizeof(parts) ;
    if(sz >= 1) {
        gmcp.package = parts[0] ;
        if(sz >= 2) {
            if(sz == 2)
                gmcp.module = parts[1] ;
            else if(sz == 3) {
                gmcp.module = parts[1] ;
                gmcp.submodule = parts[2] ;
            }
        }
    }

    if(!gmcp.package)
        return null ;
    if(!gmcp.module)
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

void init_gmcp(object who) {
    GMCP_D->send_gmcp(who, GMCP_PKG_CHAR_STATUSVARS) ;
    GMCP_D->send_gmcp(who, GMCP_PKG_CHAR_STATUS) ;
}
