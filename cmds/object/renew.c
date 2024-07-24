/**
 * @file /cmds/object/renew.c
 * @description Command for renewing objects. Destructing them, and recloning
 *              them.
 *
 * @created 2024/07/23 - Gesslar
 * @last_modified 2024/07/23 - Gesslar
 *
 * @history
 * 2024/07/23 - Gesslar - Created
 */

#include <move.h>

inherit STD_CMD ;

mixed main(object tp, string str) {
    object ob, env ;
    string e, file ;

    if(!str)
        return _info("Usage: renew <object>") ;

    if(!ob = get_object(str))
        return _error("No object found with that name.") ;

    if(ob->is_room())
        return _error("You cannot renew rooms.") ;

    if(ob->no_renew())
        return _error("That object cannot be renewed.") ;

    if(!clonep(ob))
        return _error("That object is not a clone. Use update instead.") ;

    if(virtualp(ob))
        return _error("You cannot renew virtual objects.") ;

    env = environment(ob) ;

    if(ob->move(VOID_ROOM) != MOVE_OK)
        return _error("Failed to move the object to the void.") ;

    file = base_name(ob) ;
    tp->set_env("cwf", file) ;
    ob->remove() ;

    // Destruct the original master file
    e = catch(file->remove()) ;
    if(e)
        return _error("Failed to destruct the original object: " + e) ;

    e = catch(load_object(file)) ;
    if(e)
        return _error("Failed to reload the object: " + e) ;

    e = catch(ob = new(file)) ;
    if(e)
        return _error("Failed to renew the object: " + e) ;

    // bit-shifted return of move needs to be & MOVE_OK
    if(!(ob->move(env) & MOVE_OK)) {
        ob->remove() ;
        return _error("Failed to move the object back to its original location.") ;
    }

    return _ok("Object renewed: %s", get_short(ob)) ;
}
