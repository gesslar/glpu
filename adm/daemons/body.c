/**
 * @file /adm/daemons/body.c
 * @description Handles body instantiation
 *
 * @created 2024/07/28 - Gesslar
 * @last_modified 2024/07/28 - Gesslar
 *
 * @history
 * 2024/07/28 - Gesslar - Created
 */

inherit STD_DAEMON ;

object create_body_basic(object user) ;
object create_body(object user) ;
mixed create_ghost(object user) ;
mixed revive(object ghost, object user) ;

object create_body_basic(object user) {
    object body ;
    string err;
    string type ;
    string dest ;

    if(adminp(user))
        type = "admin" ;
    else if(devp(user))
        type = "dev" ;
    else
        type = "player" ;

    dest = sprintf("/%s/%s", type, query_privs(user)) ;
    _debug("Dest: %s", dest) ;

    err = catch(body = load_object(dest)) ;

    if(err) {
        error("Error [login]: There was an error creating your mobile.\n" +
          "\tBody: " + user->query_body_path() + "\n" + err);

        return 0 ;
    }

    body->set_name(query_privs(user));
    user->set_body(body);
    set_privs(body, body->query_proper_name());
    body->restore_user();

    return body;
}

object create_body(object user) {
    string err;
    object body ;

    body = create_body_basic(user);
    if(!body)
        return 0;

    if(body->is_dead())
        body = create_ghost(user);

    body->setup_body() ;
    body->set_user(user);

    return body;
}

mixed create_ghost(object user) {
    string err ;
    object ghost ;
    string name ;

    name = query_privs(user) ;
    err = catch(ghost = load_object(sprintf("/ghost/%s", query_privs(user)))) ;
    if (err) {
        log_file("ghost", err) ;
        return err ;
    }

    ghost->set_name(name) ;
    user->set_body(ghost) ;
    set_privs(ghost, name) ;

    return ghost ;
}
