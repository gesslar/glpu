#include <simul_efun.h>
#include <type.h>

// gently stolen from atari2600tim
// https://github.com/atari2600tim/LPCsnippets/blob/master/uuid.c
/**
 * @simul_efun generate_uuid
 * @description Generates a random "version 4" UUID. Version 4 UUIDs are based
 *              on random numbers.
 * @returns {string} - A randomly generated version 4 UUID.
 */
string generate_uuid() {
    //This is a "version 4" uuid.  This is based on random numbers, the others use time.
    // https://en.wikipedia.org/wiki/Universally_unique_identifier#Version_4_(random)
    // supposed to use lower case but accept upper case (so run lower_case when comparing incoming stuff)
    return sprintf("%04x%04x-%04x-4%03x-%0x%03x-%04x%04x%04x",
        random(0x10000),random(0x10000),random(0x10000),random(0x1000),
        (8+random(4)), random(0x1000),random(0x10000),random(0x10000),random(0x10000)) ;
}

/**
 * @simul_efun of
 * @description Checks if a value is present in an array, mapping, or string.
 * @param {mixed} needle - The value to search for.
 * @param {mixed} haystack  - The array, mapping, or string to search in.
 * @returns {int} - 1 if the value is found, 0 otherwise.
 */
int of(mixed needle, mixed haystack) {
    switch(typeof(haystack)) {
        case T_ARRAY:
            return member_array(needle, haystack) > -1 ;
        case T_MAPPING:
            return !nullp(haystack[needle]) ;
        case T_STRING:
            return strsrch(haystack, needle) > -1 ;
        default:
            return 0 ;
    }
}
