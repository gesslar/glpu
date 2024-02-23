#include "/adm/obj/simul_efun.h"

//:FUNCTION append
//Append a string to another string if it is not already there.
//If the string is already there, the original string is returned.
//Otherwise, the original string with the new string appended is returned.
//
// arg1 - source - the string to append to
// arg2 - to_append - the string to append
string append(string source, string to_append) {
    int source_len, to_append_len;

    source_len = strlen(source);
    to_append_len = strlen(to_append);

    if(source[<to_append_len..] != to_append) {
        source += to_append;
    }

    return source;
}

//:FUNCTION prepend
//Prepend a string to another string if it is not already there.
//If the string is already there, the original string is returned.
//Otherwise, the original string with the new string prepended is returned.
//
// arg1 - source - the string to prepend to
// arg2 - to_prepend - the string to prepend
string prepend(string source, string to_prepend) {
    int source_len, to_prepend_len;

    source_len = strlen(source);
    to_prepend_len = strlen(to_prepend);

    if(source[0..to_prepend_len-1] != to_prepend) {
        source = to_prepend + source;
    }

    return source;
}

//:FUNCTION chop
//Chop a string off the end of another string if it is there.
//If the string is not there, the original string is returned.
//Otherwise, the original string with the new string chopped off is returned.
//
// arg1 - str - the string to chop
// arg2 - sub - the string to chop off
// arg3 - dir - the direction to chop, 1 for left (optional), -1 for right
varargs string chop(string str, string sub, int dir) {
    int sub_len;

    if(nullp(str)) error("chop: Missing argument 1 for chop");
    if(nullp(sub)) error("chop: Missing argument 2 for chop");

    if(dir != -1)
        dir = 1;

    sub_len = strlen(sub);
    if(dir == 1) {
        if(sub[0..sub_len-1] == sub) {
            str = str[0..sub_len];
        }
    } else if(dir == -1) {
        if(str[<sub_len..] == sub) {
            str = str[0..<sub_len+1];
        }
    }

    return str;
}

//:FUNCTION extract
//Extract a substring from a string.
//
// arg1 - str - the string to extract from
// arg2 - from - the starting position to extract from
// arg3 - to - the ending position to extract to (optional)
varargs string extract(string str, int from, int to) {
    if(nullp(to)) return str[from ..];
    else return str[from .. to];
}

//:FUNCTION no_ansi
//Return a string with all ANSI colour codes removed.
//
// arg1 - str - the string to remove ANSI codes from
string no_ansi(string str) {
    return XTERM256->substitute_colour(str, "plain") ;
}

//:FUNCTION simple_list
//Return a string that is a simple list of the elements of the array.
//The conjunction is the word used to join the last two elements of the list.
//If the array is empty, an error is raised.
//
// arg1 - arr - the array to make a list from
// arg2 - conjunction - the word to join the last two elements of the list
varargs string simple_list(string *arr, string conjunction) {
    if(!arr) error("simple_list: Missing argument 1 for simpple_list");
    if(!conjunction) conjunction = "and";

    if(sizeof(arr) == 0) error("simple_list: Argument 1 for simple_list is empty");
    else if(sizeof(arr) == 1) return arr[0];
    else if(sizeof(arr) == 2) return arr[0] + " " + conjunction + " " + arr[1];
    else return implode(arr[0..<2], ", ") + ", " + conjunction + " " + arr[<1];
}
