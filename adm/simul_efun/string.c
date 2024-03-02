#include "/adm/obj/simul_efun.h"
#include <type.h>

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

//:FUNCTION substr
//Return a substring of a string, starting from 0 and ending at the first
//occurrence of another string within it.
//
//If the reverse flag is set, the substring will start at the last occurrence
//of the substring within the string, skipping over the whole substring, and
//ending at the end of the string.
//
// arg1 - str - the string to extract from
// arg2 - sub - the substring to extract to
// arg3 - reverse - if set, the substring will start at the last occurrence
//        (optoinal)
varargs string substr(string str, string sub, int reverse) {
    int sub_len;
    int pos ;

    if(nullp(str)) error("substr: Missing argument 1 for substr");
    if(nullp(sub)) error("substr: Missing argument 2 for substr");

    reverse = !!reverse ;
    sub_len = strlen(sub);
    pos = strsrch(str, sub, reverse) ;

    if(pos == -1) return "" ;

    if(reverse)
        return str[pos+sub_len..] ;
    else
        return str[0..pos-1] ;
}

// This simul_efun blatantly (but lovingly) ripped off from:
// https://github.com/fluffos/dead-souls/blob/09a74caa87d8aadbfe303c294cc0bebb25fdb4db/lib/secure/sefun/strings.c#L104C1-L242C1
varargs mixed from_string(string str, int flag) {
    mixed *ret = ({ 0, "" });

    if(!str || !sizeof(str) || str == "") return 0;

    if( (str = trim(str)) == "" ) return 0;
    if( str[0] == '(' ) {
        switch(str[1]) {
            case '{':
                ret[0] = ({});
                str = str[2..];
                while(str[0] != '}') {
                    mixed *tmp;

                    tmp = from_string(str, 1);
                    ret[0] += ({ tmp[0] });
                    str = tmp[1];
                    while(str[0] == ' ' || str[0] == '\t') str = str[1..];
                    if( str[0] != ',' && str[0] != '}' )
                    error("Improperly formatted array: " + str + "\n");
                    else if( str[0] == ',') {
                        str = str[1..];
                        while(str[0] == ' ' || str[0] == '\t') str = str[1..];
                    }
                }
                if( str[1] != ')' ) {
                    str = str[2..];
                    while(str[0] == ' ' || str[0] == '\t') str = str[1..];
                    if( str[0] != ')' ) error("Illegal array terminator.\n");
                    else ret[1] = str[1..];
                }
                else ret[1] = str[2..];
                if( !flag ) return ret[0];
                while(ret[1][0] == ' ' || ret[1][0] == '\t') ret[1] = ret[1][1..];
                return ret;
            case '[':
                ret[0] = ([]);
                str = str[2..];
                while(str[0] != ']') {
                    mixed *tmp;
                    mixed cle;

                    tmp = from_string(str, 1);
                    str = tmp[1];
                    while(str[0] == ' ' || str[0] == '\t') str = str[1..];
                    if( str[0] != ':' )
                        error("Illegally formatting mapping: " + str + "\n");
                    cle = tmp[0];
                    tmp = from_string(str[1..], 1);
                    ret[0][cle] = tmp[0];
                    str = tmp[1];
                    while(str[0] == ' ' || str[0] == '\t') str = str[1..];
                    if( str[0] != ',' && str[0] != ']' )
                        error("Illegally formatted mapping: " + str + "n");
                    else if( str[0] != ']' ) {
                        str = str[1..];
                        while(str[0] == ' ' || str[0] == '\t') str = str[1..];
                    }
                }
                if( str[1] != ')' ) {
                    str = str[2..];
                    while(str[0] == ' ' || str[0] == '\t') str = str[1..];
                    if( str[0] != ')' ) error("Illegal array terminator.\n");
                    else ret[1] = str[1..];
                }
                else ret[1] = str[2..];
                if( !flag ) return ret[0];
                while(ret[1][0] == ' ' || ret[1][0] == '\t') ret[1] = ret[1][1..];
                return ret;
        }
    } else if( str[0] == '"' ) {
        string tmp;

        tmp = "";
        while( str[1] != '"' || (str[1] == '"' && str[0] == '\\') ) {
            if( str[1] == '"' ) tmp = tmp[0..<2] + "\"";
            else tmp += str[1..1];
            str = str[1..];
        }
        if( !flag ) return tmp;
        if( strlen(str) > 2 ) str = trim(str[2..]);
        return ({ tmp, str });
    } else if( str[0] >= '0' && str[0] <= '9' || str[0] == '-' ) {
        string tmp;
        int y;

        if( strlen(str) > 1 && str[0] == '-' ) {
            tmp = str[0..0];
            str = str[1..];
        } else {
            tmp = "";
        }
        if( strlen(str) > 1 && str[0..1] == "0x" ) {
            tmp = "0x";
            str = str[2..];
            while(str != "" && (str[0] >= '0' && str[0] <= '9')) {
                tmp += str[0..0];
                if( strlen(str) > 1 ) str = str[1..];
                else str = "";
            }
            sscanf(tmp, "%x", y);
        } else {
            while(str != "" && (str[0] >= '0' && str[0] <= '9')) {
                tmp += str[0..0];
                if( strlen(str) > 1 ) str = str[1..];
                else str = "";
            }
            sscanf(tmp, "%d", y);
        }
        if( !flag ) return y;
        if( str != "" ) str = trim(str);
        return ({ y, str });
    } else {
        string tmp;

        tmp = "";
        while(strlen(str) && ((str[0] >= 'a' && str[0] <= 'z') ||
                    (str[0] >= 'A' && str[0] <= 'Z') ||
                    (str[0] >= '0' && str[0] <= '9') ||
                    (str[0] == '_'))) {
            tmp += str[0..0];
            if( strlen(str) > 1 ) str = str[1..];
            else str = "";
        }
        if( !flag ) return get_objects(tmp);
        else return ({ get_objects(tmp), str });
    }
    error("Gobbledygook in string.\n");
}

// private nosave string decimal = mud_config("DECIMAL") ;
// private nosave string thousands = mud_config("THOUSANDS") ;
private nosave string decimal = "." ;
private nosave string thousands = "," ;

//:FUNCTION add_commas
//Return a string with commas added to the number.
//
// arg1 - number - the number to add commas to
string add_commas(mixed number) {
    string num_str ;
    string result = "";
    int len ;
    int dot_index ;
    int insert_position;

    if(typeof(number) == T_INT) {
        num_str = sprintf("%d", number);
    } else if(typeof(number) == T_FLOAT) {
        num_str = sprintf("%f", number);
    } else if(typeof (number) == T_STRING) {
        if((dot_index = strsrch(number, decimal)) > -1) {
            string int_part = number[0..dot_index-1] ;
            return sprintf("%s.%s", add_commas(to_int(int_part)), number[dot_index+1..]) ;
        }
        else
            number = to_int(number);
        if(nullp(number))
            error("add_commas: Argument 1 must be an number, or a string that can be converted to a number.") ;
        return add_commas(number);
    } else {
        error("add_commas: Argument 1 must be an number, or a string that can be converted to a number.") ;
    }

    len = strlen(num_str);
    dot_index = strsrch(num_str, decimal, -1); // Search for decimal point from the end

    // If there's a decimal point, handle the fractional part separately
    if (dot_index != -1) {
        result = add_commas(to_int(num_str[0..dot_index-1])); // Recurse on the integer part
        result = sprintf("%s%s%s", result, decimal, num_str[dot_index+1..]);
        return result;
    }

    // Calculate where to start inserting commas
    insert_position = (len % 3) || 3;

    for (int i = 0; i < len; i++) {
        if (i == insert_position && i != 0) {
            result = result + thousands + num_str[i..i]; // Insert comma then the character
            insert_position += 3;
        } else {
            result += num_str[i..i]; // Just add the character
        }
    }

    return result;
}
