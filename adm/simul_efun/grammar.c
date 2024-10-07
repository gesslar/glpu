#include <simul_efun.h>

// https://github.com/michaelprograms/nightmare-residuum/blob/main/lib/secure/sefun/grammar.c#L18

/**
 * @simul_efun ordinal
 * @description Converts an integer to its ordinal string representation.
 * @param {int} n - The integer to convert.
 * @returns {string} - The ordinal string representation of the integer.
 */
string ordinal (int n) {
    int x ;
    if(undefinedp(n) || !intp(n)) error("Bad argument 1 to english->ordinal") ;
    if(n < 0) error("Bad argument 1 to english->ordinal") ;
    if(n < 10) {
        if(n == 9) return "ninth" ;
        if(n == 8) return "eighth" ;
        if(n == 7) return "seventh" ;
        if(n == 6) return "sixth" ;
        if(n == 5) return "fifth" ;
        if(n == 4) return "fourth" ;
        if(n == 3) return "third" ;
        if(n == 2) return "second" ;
        if(n == 1) return "first" ;
        if(n == 0) return "zeroth" ;
    } else {
        x = (n < 14 && n > 10) ? 4 : n % 10 ;
        if(x == 1) return n + "st" ;
        else if(x == 2) return n + "nd" ;
        else if(x == 3) return n + "rd" ;
        else return n + "th" ;
    }
}

// int_string.c
// Written by Domini@Ultimate (22-08-93)
// Originally called fullnum(). Mobydick@TMI-2 renamed it, and patched
// it into the mudlib, on 93-08-22.
//
// Convert a numerical number to a worded number.
/**
 * @simul_efun int_string
 * @description Converts a numerical number to its worded number representation.
 * @param {int} num - The numerical number to convert.
 * @returns {string} - The worded number representation of the integer.
 */
string int_string (int num) {
    string str ;
    int temp ;

    if(num==2147483647) return "too much" ;
    str = "" ;
    if(num < 0) {
        num = -num ;
        str += "minus " ;
    }
    if(num==2147483647) return "too much" ;
    if(num < 20) {
        switch (num) {
            case 0:  return "zero" ;
            case 1:  str += "one"; break ;
            case 2:  str += "two"; break ;
            case 3:  str += "three"; break ;
            case 4:  str += "four"; break ;
            case 5:  str += "five"; break ;
            case 6:  str += "six"; break ;
            case 7:  str += "seven"; break ;
            case 8:  str += "eight"; break ;
            case 9:  str += "nine"; break ;
            case 10: str += "ten"; break ;
            case 11: str += "eleven"; break ;
            case 12: str += "twelve"; break ;
            case 13: str += "thirteen"; break ;
            case 14: str += "fourteen"; break ;
            case 15: str += "fifteen"; break ;
            case 16: str += "sixteen"; break ;
            case 17: str += "seventeen"; break ;
            case 18: str += "eighteen"; break ;
            case 19: str += "nineteen"; break ;
            default: break ;
        }
        return str ;
    }
    if(num < 100) {
        temp = num/10 ;
        num -= temp*10 ;
        switch (temp) {
            case 2: str += "twenty"; break ;
            case 3: str += "thirty"; break ;
            case 4: str += "forty"; break ;
            case 5: str += "fifty"; break ;
            case 6: str += "sixty"; break ;
            case 7: str += "seventy"; break ;
            case 8: str += "eighty"; break ;
            case 9: str += "ninety"; break ;
            default: break ;
        }
        if(num>0) {
            str += "-"+int_string(num) ;
        }
        return str ;
    }
    if(num < 1000) {
        temp = num/100 ;
        num -= temp*100 ;
        str += int_string(temp)+" hundred" ;
        if(num>0) {
            str += " and "+int_string(num) ;
        }
        return str ;
    }
    if(num < 1000000) {
        temp = num/1000 ;
        num -= temp*1000 ;
        str += int_string(temp)+" thousand" ;
        if(num>0) {
            if(num<100) str += " and" ;
            str += " "+int_string(num) ;
        }
        return str ;
    }
    temp = num/1000000 ;
    num -= temp*1000000 ;
    str += int_string(temp)+" million" ;
    if(num>0) {
        if(num<100) str += " and" ;
        str += " "+int_string(num) ;
    }
    return str ;
}
#include <colour.h>

/**
 * @simul_efun consolidate
 * @description Returns a consolidated string for a given quantity and item
 *              description, handling pluralization and special cases.
 * @param {int} x - The quantity of items.
 * @param {string} str - The description of the item(s).
 * @returns {string} - The consolidated string.
 */
string consolidate(int x, string str) {
    string* words ;
    string tmp ;

    if(x == 1 || !sizeof(str)) return str ;
    words = explode(str, " ") ;
    if(sscanf(words[<1], "(%s)", tmp)) {
        if(sizeof(words) == 1)
            return "(" + consolidate(x, tmp) + ")" ;
        else return consolidate(x, implode(words[0..<2], " ")) +
            " (" + tmp + ")" ;
    }
    if(sscanf(words[<1], "[%s]", tmp)) {
        if(sizeof(words) == 1)
            return "[" + consolidate(x, tmp) + "]" ;
        else return consolidate(x, implode(words[0..<2], " ")) +
            " [" + tmp + "]" ;
    }
#if 0
    // TODO: Accommodate for new colour style
    /*
    if(words[0][0..1] == "%^") {
        string* parts ;
        string part, colour = "" ;
        int i = 0 ;

        parts = explode(words[0], "%^") ;
        if(sizeof(parts) == 1) {
            if(sizeof(words) == 1) return words[0] ;
            else return words[0] + consolidate(x, implode(words[1..], " ")) ;
        }

        foreach(part in parts) {
            if(sizeof(part) && !sizeof(strip_colours("%^" + part + "%^")))
                colour += ("%^" + part + "%^") ;
            else return colour + consolidate(x,
                    (implode(parts[i..], "%^")) + " " +
                    (implode(words[1..], " "))) ;
            i++ ;
        }
        return words[0] + " " + consolidate(x, implode(words[1..], " ")) ;

    }
    */
#endif
    if(member_array(lower_case(no_ansi(words[0])), ({"a", "an", "the", "one"})) > -1) words = words[1..] ;
    return (query_num(x) + " " + pluralize(implode(words, " "))) ;
}
