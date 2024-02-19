// /std/modules/clean.c
// Clean up for daemons
//
// Created:     2022/08/24: Gesslar
// Last Change: 2022/08/24: Gesslar
//
// 2022/08/24: Gesslar - Created

#include <clean.h>

// Variables
private nosave int no_clean_up = 1 ;

// Functions
int can_clean_up() ;
int set_no_clean(int no_clean) ;
int query_no_clean() ;
private int clean_up_check(mixed ob) ;
int request_clean_up() { return 1 ; }

// Functions from other objects
int is_daemon() ;
int is_command() ;

/*protected*/ int clean_up(int refs) {
    object *contents ;

    // If we have an environment, straight up don't ask again. We can never
    // lose our environment and only non-environmented things are cleaned up.
    // Things with an environment rely on their environment to clean them up.
    if(environment()) return CLEAN_NEVER_AGAIN ;

    // Never clean up if:
    // * We are a user
    // * We are interactive
    // * We have the no_clean_up flag set
    if(clean_up_check(this_object()) > 0)
        return CLEAN_NEVER_AGAIN ;

    // Now ask permission to clean up. If we answer false, we'll check again
    // later.
    if(request_clean_up() == 0) return CLEAN_LATER ;

    // If we have more than one reference to use, such as when we're cloned,
    // clean up later when we have no references. We don't even check for
    // virtual items, because refs is weird with them.
    if(clonep())
        if(refs > 1)
            if(!virtualp())

    // If we are a command or we are a daemon, we need to not clean up if
    // we have any pending call_outs. We'll try again later.
    if(!is_command() && !is_daemon()) {
        mixed *calls ;

        calls = call_out_info() ;
        calls = filter(calls, (: $1 == $2 :), this_object()) ;

        if(sizeof(calls)) return CLEAN_LATER ;
    }

    // If we're a room, or a container, or anything of the like, we need to
    // determine if we have any items in us that need to be cleaned up.
    contents = deep_inventory() ;
    if(clean_up_check(contents) > 0) return CLEAN_LATER ;
    contents = filter(contents, (: $1->request_clean_up() == 0 :)) ;
    if(sizeof(contents)) return CLEAN_LATER ;

    // We're definitely cleaning up now. We'll tell everyone we're cleaning
    // up, and then we'll remove ourselves.
    event(this_object(), "cleaning_up") ;

    contents = all_inventory() ;
    filter(contents, (: destruct :)) ;
    contents -= ({ 0 }) ;
    contents->move(VOID_OB) ;

    destruct() ;

    return CLEAN_NEVER_AGAIN ;
}

int can_clean() {
    return request_clean_up() ;
}

int set_no_clean(int no_clean: (: 1 :)) {
    no_clean_up = !!no_clean ;

    return no_clean_up ;
}

int query_no_clean() {
    return no_clean_up ;
}

private int clean_up_check(mixed ob) {
    if(objectp(ob)) ob = ({ ob }) ;
    if(!pointerp(ob)) return 0 ;

    ob = filter(ob, (:
        objectp($1) &&
        $1->query_no_clean() == 1 &&
        (userp($1) ||
        interactive($1))
    :) ) ;

    return sizeof(ob) ;
}
