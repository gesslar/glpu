#include "/adm/obj/simul_efun.h"

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
