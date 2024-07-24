/**
 * @file /adm/daemons/virtual.c
 * @description Daemon for virtual objects
 *
 * @created 2024/02/04 - Gesslar
 * @last_modified 2024/02/04 - Gesslar
 *
 * @history
 * 2024/02/04 - Gesslar - Created
 */

inherit STD_DAEMON ;

public nomask object compile_object(string file) {
    string *path_components, vclass, e ;
    string *dir_parts ;
    string module ;
    object result ;

    path_components = dir_file(file);

    dir_parts = explode(path_components[0], "/") - ({""});
    vclass = dir_parts[0] ;

    module = sprintf("%smodules/virtual/%s.c", __DIR__, vclass) ;

    if(file_exists(module)) {
        e = catch(load_object(module)) ;
        if(e) {
            log_file("VIRTUAL", e) ;
            return 0 ;
        }

        e = catch(result = call_other(sprintf(module), "compile_object", file)) ;

        if(e) {
            log_file("VIRTUAL", e) ;
            return 0 ;
        }

        return result ;
    }

    return 0 ;
}
