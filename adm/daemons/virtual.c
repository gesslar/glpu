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
inherit M_LOG ;

void setup() {
    set_log_level(4) ;
}

public nomask object compile_object(string file) {
    string *path_components, vclass, e ;
    string *dir_parts ;
    object result ;

    path_components = dir_file(file);

    dir_parts = explode(path_components[0], "/") - ({""});
    vclass = dir_parts[0] ;

    if(file_exists(__DIR__ "modules/virtual/%s.c", vclass)) {
        e = catch(load_object(sprintf(__DIR__ "modules/virtual/%s", vclass))) ;
        if(e) {
            log_file("VIRTUAL", e) ;
            return 0 ;
        }

        e = catch(result = call_other(sprintf(__DIR__ "modules/virtual/%s", vclass), "compile_object", file)) ;

        if(e) {
            log_file("VIRTUAL", e) ;
            return 0 ;
        }

        return result ;
    }

    return 0 ;
}
