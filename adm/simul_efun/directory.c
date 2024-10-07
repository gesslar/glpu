#include <simul_efun.h>

// mudlib: basis
// date:   1992/09/05
// author: Truilkan

/**
 * @simul_efun assure_dir
 * @description Ensures that a directory exists by creating it and its parent
 *              directories if necessary.
 * @param {string} path - The path of the directory to ensure.
 * @returns {int} - 1 if the directory exists or was created successfully,
 *                  otherwise 0.
 */
mixed assure_dir(string path) {
    string *parts, dir ;
    int j ;

    string old_privs ;

    if(file_size(path) == -2) return 1 ;

    old_privs = query_privs() ;
    set_privs(this_object(), query_privs(previous_object())) ;

    parts = explode(path, "/") ;
    dir = "" ;
    for(j = 0; j < sizeof(parts); j++) {
        dir += parts[j] ;
        mkdir(dir) ;
        dir += "/" ;
    }

    set_privs(this_object(), old_privs) ;

    return file_size(path) == -2 ;
}

/**
 * @simul_efun query_directory
 * @description Returns the directory of the given object. If no object is
 *              provided, it defaults to the previous object.
 * @param {object} ob - The object to query the directory of.
 * @returns {string} - The directory path of the object.
 */
string query_directory(object ob) {
    string file, *parts ;
    string dir ;

    if(!objectp(ob))
        ob = previous_object() ;

    if(!objectp(ob))
        error("Bad argument 1 to query_directory()") ;

    file = base_name(ob) ;
    parts = explode(file, "/") ;
    parts = parts[0..<2] ;
    dir = implode(parts, "/") ;

    return append(prepend(dir, "/"), "/") ;
}
