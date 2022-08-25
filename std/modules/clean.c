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
int can_clean() ;
int set_no_clean(int no_clean) ;

protected int clean_up(int inherited)
{
    mixed *calls ;

    if(no_clean_up == 1) return CLEAN_NEVER_AGAIN ;

    if(clonep())
    {
        if(inherited > 1)
        {
            return CLEAN_LATER ;
        }
    }

    calls = call_out_info() ;
    calls = filter(calls, (: $1 == $2 :), this_object()) ;

    if(sizeof(calls)) return CLEAN_LATER ;

    call_other(this_object(), "remove") ;
    if(objectp(this_object())) destruct(this_object()) ;

    return CLEAN_NEVER_AGAIN ;
}

int can_clean()
{
    return !no_clean_up ;
}

int set_no_clean(int no_clean)
{
    if(nullp(no_clean)) no_clean = 1 ;

    if(no_clean != 0 && no_clean != 1) error("Invalid argument 1 to set_no_clean") ;

    no_clean_up = no_clean ;

    return no_clean_up ;
}
