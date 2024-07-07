/**
 * @file /adm/daemons/crash.c
 * @description This daemon gets called by the master object when the mud
 * crashes in order for registered obects to save their data.
 *
 * @created 2024/07/06 - Gesslar
 * @last_modified 2024/07/06 - Gesslar
 *
 * @history
 * 2024/07/06 - Gesslar - Created
 */

#include <daemons.h>

inherit STD_DAEMON ;

private nosave object *crash_objects = ({}) ;

void setup() {
    set_no_clean(1) ;
    crash_objects = ({}) ;
}

/**
 * @description Register an object to be called when the mud crashes.
 * @param obj The object to register.
 */

void register_crash_object(object obj) {
    if(member_array(obj, crash_objects) == -1)
        crash_objects += ({ obj }) ;
}

/**
 * @description Unregister an object from being called when the mud crashes.
 * @param obj The object to unregister.
 */

void unregister_crash_object(object obj) {
    if(member_array(obj, crash_objects) != -1)
        crash_objects -= ({ obj }) ;
}

/**
 * @description Query the objects registered to be called when the mud crashes.
 * @return The objects registered to be called when the mud crashes.
 */

object *query_crash_objects() {
    return crash_objects ;
}

/**
 * @description Call the registered objects to save their data.
 */

void crashing() {
    crash_objects -= ({ 0 }) ;
    foreach(object obj in crash_objects) {
        debug("Crashing: %O", obj) ;
        catch(obj->crash()) ;
    }
}
