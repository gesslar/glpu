#include "/adm/obj/simul_efun.h"

string object_save_directory(object ob) {
    string source_dir = query_directory(ob) ;
    string save_dir ;

    save_dir = OB_DATA_DIR + source_dir[1..] + "/" ;

    return save_dir ;
}

string object_save_file(object ob) {
    string source_dir = object_save_directory(ob) ;
    string source_file = query_file_name(ob) ;
    string save_file ;

    save_file = source_dir + source_file;

    return save_file ;
}

string assure_object_save_dir(object ob) {
    string dir = object_save_directory(ob) ;

    if(directory_exists(dir))
        return dir ;

    mkdirs(dir) ;

    if(directory_exists(dir))
        return dir ;

    return 0 ;
}
