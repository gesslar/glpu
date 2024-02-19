#include "/adm/obj/simul_efun.h"

// mudlib: basis
// date:   1992/09/05
// author: Truilkan
int mkdirs(string path) {
    string *parts, dir;
    int j;

    string old_privs ;

    if(file_size(path) == -2) return 1 ;

    old_privs = query_privs(this_object()) ;
    set_privs(this_object(), query_privs(previous_object())) ;

    parts = explode(path, "/");
    dir = "";
    for (j = 0; j < sizeof(parts); j++) {
        dir += parts[j];
        mkdir(dir);
        dir += "/";
    }

    set_privs(this_object(), old_privs) ;

    return file_size(path) == -2;
}

string query_directory(object ob) {
    string file, *parts;
    string dir ;

    if(!objectp(ob))
        ob = previous_object() ;

    if(!objectp(ob))
        error("Bad argument 1 to query_directory()") ;

    file = base_name(ob) ;
    parts = explode(file, "/");
    parts = parts[0..<2];
    dir = implode(parts, "/");

    return append(prepend(dir, "/"), "/");
}
