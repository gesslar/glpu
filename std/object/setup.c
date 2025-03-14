/**
 * @file /std/modules/setup.c
 * @description This is the setup chain for objects.
 *
 * @created 2024-01-29 - Gesslar
 * @last_modified 2024-01-29 - Gesslar
 *
 * @history
 * 2024-01-29 - Gesslar - Created
 */

varargs void setup_chain(mixed args...) {
    int x;

    // Itended for objects at their very mudlib level
    call_if(this_object(), "mudlib_setup", args...);

    // Intended for objects that that are base for things like rooms of
    // a common zone, room-type, or anything else that needs to setup
    // the base of its object, after the mudlib_setup has been called.
    call_if(this_object(), "base_setup", args...);

    x = 5;
    for(x = 0; x < 5; x++)
        call_if(this_object(), "pre_setup_"+x, args...);

    // The actual setup function that is ordinarily used by any object
    // that isn't the master or a base object.
    call_if(this_object(), "setup", args...);

    x = 5;
    for(x = 0; x < 5; x++)
        call_if(this_object(), "post_setup_"+x, args...);

    if(call_if(this_object(), "query_persistent")) {
        call_if(this_object(), "restore_data");
        call_if(this_object(), "post_restore");
    }

    call_if(this_object(), "restore_data");
    call_if(this_object(), "post_restore");

    call_if(this_object(), "mudlib_complete_setup");
}

// exactly the same as setup_chain()
void unsetup_chain() {
    int x;

    // Itended for objects at their very mudlib level
    call_if(this_object(), "mudlib_unsetup", previous_object(1));

    // Intended for objects that that are base for things like rooms of
    // a common zone, room-type, or anything else that needs to unsetup
    // the base of its object, after the mudlib_unsetup has been called.
    call_if(this_object(), "base_unsetup", previous_object(1));

    for(x = 0; x < 5; x++)
        call_if(this_object(), "pre_unsetup_"+x, previous_object(1));

    // The actual unsetup function that is ordinarily used by any object
    // that isn't the master or a base object.
    call_if(this_object(), "unsetup", previous_object(1));

    for(x = 0; x < 5; x++)
        call_if(this_object(), "post_unsetup_"+x, previous_object(1));

    if(call_if(this_object(), "query_persistent")) {
        call_if(this_object(), "save_data");
        call_if(this_object(), "post_save");
    }
}

// same as above but for virtual
varargs void virtual_setup_chain(mixed args...) {
    int x;

    call_if(this_object(), "virtual_mudlib_setup", args...);

    for(x = 0; x < 5; x++)
        call_if(this_object(), "pre_virtual_setup_" + x, args...);

    call_if(this_object(), "virtual_setup", args...);

    x = 5;
    while(x--)
        call_if(this_object(), "post_virtual_setup_" + x, args...);

    call_if(this_object(), "virtual_mudlib_complete_setup");
}
