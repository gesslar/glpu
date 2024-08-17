/**
 * @file /adm/daemons/body.c
 * @description Handles body instantiation
 *
 * @created 2024-07-28 - Gesslar
 * @last_modified 2024-07-28 - Gesslar
 *
 * @history
 * 2024-07-28 - Gesslar - Created
 */

inherit STD_DAEMON ;

object create_body_basic(object user) ;
object create_body(object user) ;
mixed create_ghost(object user) ;
mixed revive(object ghost, object user) ;

object create_body(string name) {
    object body ;
    string err;
    string type ;
    string dest ;

    if(!name) {
        _debug("create_body: No name provided") ;
        return 0 ;
    }

    name = lower_case(name) ;

    if(!user_exists(name)) {
        _debug("create_body: User does not exist") ;
        return 0 ;
    }

    if(adminp(name)) {
        _debug("create_body: User is admin") ;
        type = "admin" ;
    }
    else if(devp(name)) {
        _debug("create_body: User is dev") ;
        type = "dev" ;
    }
    else {
        _debug("create_body: User is player") ;
        type = "player" ;
    }

    dest = sprintf("/%s/%s", type, name) ;

    err = catch(body = load_object(dest)) ;

    if(err) {
        _debug("create_body: Error loading object") ;
        return 0 ;
    }

    if(!body) {
        _debug("create_body: Body does not exist") ;
        return 0 ;
    }

    body->set_name(name) ;
    set_privs(body, name) ;
    body->restore_body();

    return body;
}

mixed create_ghost(string name) {
    string err ;
    object ghost ;

    if(!name)
        return 0 ;

    name = lower_case(name) ;

    err = catch(ghost = load_object(sprintf("/ghost/%s", name))) ;
    if(err) {
        log_file("ghost", err) ;
        return err ;
    }

    ghost->set_name(name) ;
    set_privs(ghost, name) ;

    return ghost ;
}
