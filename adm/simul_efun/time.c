#include "/adm/obj/simul_efun.h"

/**
 * @simul_efun ldate
 * @description Returns a formatted date string.
 * @param {int} [x=time()] - The timestamp to format. Defaults to the current time.
 * @param {int} [brief=0] - If set to 1, returns a brief date format (MM-DD).
 *                          Otherwise, returns a full date format (YYYY-MM-DD).
 * @returns {string} The formatted date string.
 */
varargs string ldate(int x, int brief) {
    string fmt ;

    if(nullp(x)) x = time() ;

    if(x == 1) {
        brief = 1 ;
        x = time() ;
    }

    if(brief) fmt = "%m-%d" ;
    else fmt = "%Y-%m-%d" ;

    return strftime(fmt, x) ;
}

/**
 * @simul_efun ltime
 * @description Returns a formatted time string.
 * @param {int} [x=time()] - The timestamp to format. Defaults to the current time.
 * @param {int} [brief=0] - If set to 1, returns a brief time format (HH:MM).
 *                          Otherwise, returns a full time format (HH:MM:SS).
 * @returns {string} The formatted time string.
 */
varargs string ltime(int x, int brief) {
    string fmt ;

    if(nullp(x)) x = time() ;

    if(x == 1) {
        brief = 1 ;
        x = time() ;
    }

    if(brief) fmt = "%H:%M" ;
    else fmt = "%H:%M:%S" ;

    return strftime(fmt, x) ;
}
