/**
 * @file /std/user/module.c
 * @description Module management for user objects
 *
 * @created 2024/07/29 - Gesslar
 * @last_modified 2024/07/29 - Gesslar
 *
 * @history
 * 2024/07/29 - Gesslar - Created
 */

#include <module.h>

private nosave mapping modules = ([]);

varargs object add_module(string module, mixed args...) {
    object ob ;
    string path ;
    string name ;

    if(!module || module == "") error("Error [add_module]: Invalid module name.\n") ;
    if(modules[module]) error("Error [add_module]: Module " + module + " already exists.\n") ;

    path = "/std/modules/mobile/" + module + ".c" ;
    path = replace_string(path, " ", "_") ;
    if(!file_exists(path))
        error("Error [add_module]: Module " + module + " does not exist.\n") ;

    catch(ob = new(path)) ;
    if(!ob)
        error("Error [add_module]: Module " + module + " failed to load.\n") ;

    if(ob->attach(this_object(), args...) == 0) {
        ob->remove() ;
        return 0 ;
    }

    name = ob->query_name() ;

    modules[name] = ob ;

    return ob ;
}

object query_module(string module) {
    if(!module || module == "")
        error("Error [query_module]: Invalid module name. " + module) ;
    if(!modules[module])
        return 0 ;

    return modules[module] ;
}

void remove_module(string module) {
    object ob ;

    if(!module || module == "") error("Error [remove_module]: Invalid module name.\n") ;
    if(!modules[module]) error("Error [remove_module]: Module " + module + " does not exist.\n") ;

    ob = modules[module] ;
    if(!objectp(ob))
        return ;

    ob->remove() ;
    map_delete(modules, module) ;
}

object get_module(string module) {
    object ob ;

    if(!module || module == "") error("Error [get_module]: Invalid module name.\n") ;
    ob = modules[module] ;

    if(!objectp(ob))
        return 0 ;

    return modules[module] ;
}

mapping query_modules() {
    return copy(modules) ;
}

varargs mixed module(string module, string func, mixed args...) {
    object ob ;

    if(!stringp(module) || module == "")
        error("Error [query_module]: Invalid module name.\n") ;
    if(!stringp(func) || func == "")
        error("Error [query_module]: Invalid function name.\n") ;

    ob = modules[module] ;

    if(!objectp(ob))
        return null ;

    return call_if(ob, func, args...) ;
}

void remove_all_modules() {
    foreach(string module, object ob in modules) {
        if(objectp(ob))
            catch(ob->remove()) ;
    }

    modules = ([ ]) ;
}
