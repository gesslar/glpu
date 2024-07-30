/**
 * @file /Untitled-1
 * @description su into another living. This is in cmds/std/ so that
 *              it can be used by all living objects, except only if
 *              the user is a developer.
 *
 * @created 2024/07/29 - Gesslar
 * @last_modified 2024/07/29 - Gesslar
 *
 * @history
 * 2024/07/29 - Gesslar - Created
 */

inherit STD_CMD ;

mixed main(object source, string arg) {
    object dest, su_body ;
    object user ;

    user = source->query_user() ;
    if(!devp(user))
        return 0 ;

    if(!user)
        return "Unable to find your user object.\n" ;

    if(!arg) {
        su_body = user->query_su_body() ;
        if(!su_body)
            return "You have no body to return to.\n" ;
        dest = su_body ;
    } else {
        su_body = user->query_su_body() ;
        if(su_body)
            return "You are already su'd into someone else.\n" ;

        dest = get_living(arg, environment(source)) ;
        if(!dest)
            return "No such living here.\n" ;

        if(interactive(dest))
            return "That body is already inhabited.\n" ;
    }

    if(!living(dest))
        return "That is not a living.\n" ;

    if(exec(dest, source)) {
        if(su_body) {
            if(!user->clear_su_body()) {
                exec(source, dest) ;
                return "Failed to clear your su body.\n" ;
            }
        } else {
            if(!user->set_su_body(source, dest)) {
                exec(source, dest) ;
                return "Failed to set your su body.\n" ;
            }
        }

        if(!user->set_body(dest)) {
            exec(source, dest) ;
            user->clear_su_body() ;
            return "Failed to set your body.\n" ;
        }

        if(su_body == dest) {
            tell(dest, "You return to your body.\n") ;
            dest->other_action("$O $vreturn to $p body.", source) ;
        } else {
            tell(dest, "You possess " + dest->query_name() + ".\n") ;
            dest->other_action("$O $vpossess $n.", source) ;
        }
    } else
        tell(source, "Failed to su into " + dest->query_name() + ".\n") ;

    return 1 ;
}
