#include <simul_efun.h>

/**
 * @function data_index
 * @description Finds the index of the line in the given array that starts with
 *              the specified key.
 * @param {string[]} lines - The array of strings to search through.
 * @param {string} key - The key to search for at the start of each line.
 * @returns {int} - The index of the line that starts with the key, or -1 if not found.
 */
private int data_index(string *lines, string key) {
    string *parts ;
    int i, sz ;

    for(i = 0, sz = sizeof(lines); i < sz; i++) {
        if(strsrch(lines[i], key) == 0)
            return i ;
    }

    return -1 ;
}

/**
 * @simul_efun data_value
 * @description Retrieves the value associated with a given key from a file.
 * @param {string} file - The file to read from.
 * @param {string} key - The key to search for.
 * @param {mixed} [def] - The default value to return if the key is not found.
 * @returns {mixed} - The value associated with the key, or the default value if
 *                    the key is not found.
 */
varargs mixed data_value(string file, string key, mixed def) {
    string *lines, line ;
    mixed *parts ;
    int i, sz ;

    if(nullp(file) || nullp(key))
        return null ;

    if(!file_exists(file))
        return null ;

    key = append(key, "|") ;
    lines = explode_file(file) ;
    i = data_index(lines, key) ;
    if(i == -1)
        return def ;

    line = lines[i] ;
    parts = explode(line, "|") ;
    parts = parts[1..] ;
    parts = map(parts, (: from_string :)) ;
    if(sizeof(parts) == 1)
        return parts[0] ;

    return parts ;
}

/**
 * @simul_efun data_write
 * @description Writes a key-value pair to a file. If the key already exists,
 *              the value is updated.
 * @param {string} file - The file to write to.
 * @param {string} key - The key to write.
 * @param {mixed} data - The value(s) to write.
 */
varargs void data_write(string file, string key, mixed data...) {
    string *lines, out ;
    mixed *parts ;
    int i, sz ;

    if(nullp(file) || nullp(key) || nullp(data))
        return ;

    parts = map(data, (: stringify :)) ;
    out = implode(parts, "|") ;
    key = append(key, "|") ;
    out = key + out ;

    lines = explode_file(file) ;
    i = data_index(lines, key) ;
    if(i > -1) {
        lines[i] = out ;
        implode_file(file, lines, 1) ;
        return ;
    }

    implode_file(file, lines, 1) ;
}

/**
 * @simul_efun data_del
 * @description Deletes the key-value pair from the file.
 * @param {string} file - The file to modify.
 * @param {string} key - The key to delete.
 * @returns {int} - 1 if the key was found and deleted, 0 otherwise.
 */
int data_del(string file, string key) {
    string *lines ;
    int i, sz ;

    if(nullp(file) || nullp(key))
        return 0 ;

    lines = explode_file(file) ;
    i = data_index(lines, key) ;
    if(i == -1)
        return 0 ;

    lines = lines[0..i-1] + lines[i+1..] ;
    if(sizeof(lines) == 0)
        rm(file) ;
    else
        implode_file(file, lines, 1) ;

    return 1 ;
}

/**
 * @simul_efun data_inc
 * @description Increments the value associated with the given key in the file
 *              by the specified amount. If the key does not exist, it is
 *              created with the increment value.
 * @param {string} file - The file to modify.
 * @param {string} key - The key to increment the value for.
 * @param {int} inc - The amount to increment by.
 * @returns {int} - The new value after incrementing.
 */
varargs int data_inc(string file, string key, int inc) {
    string *lines, line ;
    int i, val ;

    if(nullp(file) || nullp(key))
        return null ;

    lines = explode_file(file) ;
    i = data_index(lines, key) ;
    if(i == -1) {
        data_write(file, key, inc) ;
        return inc ;
    }

    key = append(key, "|") ;
    line = lines[i] ;
    sscanf(line, key + "%d", val) ;
    val += inc ;
    lines[i] = key + val ;
    implode_file(file, lines, 1) ;
}
