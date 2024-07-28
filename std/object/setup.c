/**
 * @file /std/modules/setup.c
 * @description This is the setup chain for objects.
 *
 * @created 2024/01/29 - Gesslar
 * @last_modified 2024/01/29 - Gesslar
 *
 * @history
 * 2024/01/29 - Gesslar - Created
 */

varargs void setup_chain(mixed args...) {
    int x ;

    call_if(this_object(), "mudlib_setup", args...) ;

    x = 5 ;
    while(x--)
        call_if(this_object(), "pre_setup_"+x, args...) ;

    call_if(this_object(), "setup", args...) ;

    x = 5 ;
    while(x--)
        call_if(this_object(), "post_setup_"+x, args...) ;

    if(call_if(this_object(), "query_persistent")) {
        call_if(this_object(), "restore_data") ;
        call_if(this_object(), "post_restore") ;
    }

    call_if(this_object(), "restore_data") ;
    call_if(this_object(), "post_restore") ;

    call_if(this_object(), "mudlib_complete_setup") ;
}

// exactly the same as setup_chain()
void unsetup_chain(mixed args...) {
    int x ;

    call_if(this_object(), "mudlib_unsetup", args...) ;

    x = 5 ;
    while(x--)
        call_if(this_object(), "pre_unsetup_"+x, args...) ;

    call_if(this_object(), "unsetup", args...) ;

    x = 5 ;
    while(x--)
        call_if(this_object(), "post_unsetup_"+x, args...) ;

    if(call_if(this_object(), "query_persistent")) {
        call_if(this_object(), "save_data") ;
        call_if(this_object(), "post_save") ;
    }
}

// same as above but for virtual
varargs void virtual_setup_chain(mixed args...) {
    int x;

    call_if(this_object(), "virtual_mudlib_setup", args...);

    x = 5;
    while (x--)
        call_if(this_object(), "pre_virtual_setup_" + x, args...);

    call_if(this_object(), "virtual_setup", args...);

    x = 5;
    while (x--)
        call_if(this_object(), "post_virtual_setup_" + x, args...);

    call_if(this_object(), "virtual_mudlib_complete_setup");
}
