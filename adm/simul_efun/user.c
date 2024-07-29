#include <simul_efun.h>

//user.c

//Tacitus @ LPUniversity
//Grouped on October 22nd, 2005

/**
 * @simul_efun user_data_file
 * @description Returns the file path for the user's data file.
 * @param {string} name - The user's name.
 * @returns {string} - The file path for the user's data file, or 0 if the input
 *                     is invalid.
 */
string user_data_file(string name) {
    if(!name ||!stringp(name)) return 0;
    name = lower_case((string)name);
    return("/data/users/" + name[0..0] + "/" + name + "/" + name);
}

/**
 * @simul_efun user_body_data
 * @description Returns the file path for the user's mob data file.
 * @param {string} name - The user's name.
 * @returns {string} - The file path for the user's mob data file, or 0 if the input
 *                     is invalid.
 */
string user_body_data(string name) {
    if(!name ||!stringp(name)) return 0;
    name = lower_case((string)name);
    return("/data/users/" + name[0..0] + "/" + name + "/" + name + "_body");
}

/**
 * @simul_efun user_inventory_data
 * @description Returns the file path for the user's inventory data file.
 * @param {string} name - The user's name.
 * @returns {string} - The file path for the user's inventory data file, or 0
 *                     if the input is invalid.
 */
string user_inventory_data(string name) {
    if(!name ||!stringp(name)) return 0;
    name = lower_case((string)name);
    return user_data_directory(name) + name + "_inv.txt" ;
}

/**
 * @simul_efun user_data_directory
 * @description Returns the directory path for the user's data directory based on
 *              their name.
 * @param {string} name - The user's name.
 * @returns {string} - The directory path for the user's data directory, or 0 if the
 *                     input is invalid.
 */
string user_data_directory(string name) {
    if(!name ||!stringp(name)) return 0;
    name = lower_case(name);
    return("/data/users/" + name[0..0] + "/" + name + "/");
}

/**
 * @simul_efun user_core_data_directory
 * @description Returns the directory path for the user's core data directory based
 *              on their name.
 * @param {string} name - The user's name.
 * @returns {string} - The directory path for the user's core data directory, or 0 if
 *                     the input is invalid.
 */
string user_core_data_directory(string name) {
    if(!name || !stringp(name)) return 0;
    name = lower_case(name);
    return "/data/users/" + name[0..0] + "/" + name + "/" ;
}

/**
 * @simul_efun user_path
 * @description Returns the home directory path for the user.
 * @param {string|object} name - The user's name, or the body of the user.
 * @returns {string} - The home directory path for the user.
 */
string user_path(mixed name) {
    if(objectp(name) && userp(name)) name = query_privs(name);
    if(!stringp(name)) return null ;

    return "/home/" + name[0..0] + "/" + name + "/" ;
}
