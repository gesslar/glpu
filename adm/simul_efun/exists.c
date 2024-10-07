#include <simul_efun.h>

//exists.c

//Tacitus @ LPUniversity
//02-APR-05
//Simul-Efuns

/* Updates:

   30-JUNE-05 Gwegster - added user_exists()
*/

/**
 * @simul_efun directory_exists
 * @description Checks if a directory exists.
 * @param {string} dirname - The name of the directory to check.
 * @returns {int} - 1 if the directory exists, otherwise 0.
 */
int directory_exists(string dirname) {
    return (file_size(dirname) == -2) ;
}

/**
 * @simul_efun file_exists
 * @description Checks if a file exists.
 * @param {string} file - The name of the file to check.
 * @returns {int} - 1 if the file exists, otherwise 0.
 */
int file_exists(string file) {
    return (file_size(file) >= 0) ;
}

/**
 * @simul_efun cfile_exists
 * @description Checks if a compiled file (.c) exists.
 * @param {string} file - The base name of the file to check.
 * @returns {int} - 1 if the compiled file exists, otherwise 0.
 */
int cfile_exists(string file) {
    return file_exists(append(file, ".c")) ;
}

/**
 * @simul_efun ofile_exists
 * @description Checks if a save file exists.
 * @param {string} file - The base name of the file to check.
 * @returns {int} - 1 if the save file exists, otherwise 0.
 */
int ofile_exists(string file) {
    return file_exists(append(file, __SAVE_EXTENSION__)) ;
}

/**
 * @simul_efun user_exists
 * @description Checks if a user data file exists.
 * @param {string} user - The username to check.
 * @returns {int} - 1 if the user data file exists, otherwise 0.
 */
int user_exists(string user) {
    return (file_exists("/data/users/" + user[0..0] + "/" + user + "/" + user + __SAVE_EXTENSION__)) ;
}
