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

void mudlib_setup() {}
void base_setup() {}
void custom_setup() {}
void setup() {}
void post_setup() {}
void post_restore() {}
protected void reset() {}

void setup_chain() {
    mudlib_setup() ;
    base_setup() ;
    custom_setup() ;
    setup() ;
    post_setup() ;
    if(function_exists("restore_data", this_object()))
        call_other(this_object(), "restore_data");
    post_restore() ;
}
