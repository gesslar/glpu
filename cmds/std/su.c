/**
 * @file /Untitled-1
 * @description su into another living. This is in cmds/std/ so that
 *              it can be used by all living objects, except only if
 *              the user is a developer.
 *
 * @created 2024-07-29 - Gesslar
 * @last_modified 2024-07-29 - Gesslar
 *
 * @history
 * 2024-07-29 - Gesslar - Created
 */

inherit STD_CMD ;

mixed main(object source, string arg) {
    object dest, su_body ;
    object room, freezer ;

    room = environment(source) ;
    freezer = load_object(ROOM_FREEZER) ;

    su_body = source->query_su_body() ;
    if(!arg) {
        if(su_body)
            dest = su_body ;
        else
            if(!devp(source))
                return 0 ;
            else
                return "You are not su'd into anyone or your body cannot be found." ;
    } else {
        if(!devp(source))
            return 0 ;

        if(su_body)
            return "You are already su'd into "+source->query_name()+"." ;

        if(!dest = find_living(arg))
            if(!dest = present(arg, room))
                return "Cannot find " + arg + "." ;

        if(!living(dest))
            return arg + " is not a living object." ;

        if(!environment(dest))
            return "Cannot find " + arg + "." ;

        if(dest == source)
            return "You cannot su into yourself." ;
    }

    if(interactive(dest))
        return "That body is already in use." ;

    if(exec(dest, source)) {
        if(su_body) {
            source->clear_su_body() ;
        } else {
            dest->set_su_body(source) ;
        }

        if(su_body == dest) {
            dest->move(room) ;
            tell(dest, "You return to your body.\n") ;
            dest->other_action("$N $vexit the body of $o.", source) ;
        } else {
            tell(dest, "You possess " + dest->query_name() + ".\n") ;
            dest->other_action("$O $vpossess $n.", source) ;
            source->move(freezer) ;
        }
    } else
        tell(source, "Failed to su into " + dest->query_name() + ".\n") ;

    return 1 ;
}
