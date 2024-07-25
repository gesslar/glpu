#include "/adm/obj/simul_efun.h"

/**
 * @simul_efun object_save_directory
 * @description Returns the save directory path for the specified object.
 * @param {object} ob - The object to get the save directory for.
 * @returns {string} - The save directory path for the object.
 */
string object_save_directory(object ob) {
    string source_dir = query_directory(ob) ;
    string save_dir ;

    save_dir = mud_config("OBJECT_DATA_DIR") + source_dir[1..] ;

    return save_dir ;
}

/**
 * @simul_efun object_save_file
 * @description Returns the save file path for the specified object.
 * @param {object} ob - The object to get the save file for.
 * @returns {string} - The save file path for the object.
 */
string object_save_file(object ob) {
    string source_dir = object_save_directory(ob) ;
    string source_file = query_file_name(ob) ;
    string save_file ;

    save_file = source_dir + source_file;

    return save_file ;
}

/**
 * @simul_efun assure_object_save_dir
 * @description Ensures the save directory for the specified object exists,
 *              creating it if necessary.
 * @param {object} ob - The object to ensure the save directory for.
 * @returns {string} - The save directory path for the object, or 0 if creation
 *                     failed.
 */
string assure_object_save_dir(object ob) {
    string dir = object_save_directory(ob) ;

    if(directory_exists(dir))
        return dir ;

    assure_dir(dir) ;

    if(directory_exists(dir))
        return dir ;

    return 0 ;
}
