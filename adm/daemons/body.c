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

inherit STD_DAEMON;

object create_body_basic(object user);
object create_body(object user);
mixed create_ghost(object user);
mixed revive(object ghost, object user);

object create_body(string _name) {
    object body;
    string err;
    string type;
    string dest;

    if(!_name)
        return 0;

    _name = lower_case(_name);

    if(!user_exists(_name))
        return 0;

    if(adminp(_name))
        type = "admin";
    else if(devp(_name))
        type = "dev";
    else
        type = "player";

    dest = sprintf("/%s/%s", type, _name);

    err = catch(body = load_object(dest));

    if(err)
        return 0;

    if(!body)
        return 0;

    body->set_name(_name);
    set_privs(body, _name);
    body->restore_body();

    return body;
}

mixed create_ghost(string _name) {
    string err;
    object ghost;

    if(!_name)
        return 0;

    _name = lower_case(_name);

    err = catch(ghost = load_object(sprintf("/ghost/%s", _name)));
    if(err) {
        log_file("ghost", err);
        return err;
    }

    ghost->set_name(_name);
    set_privs(ghost, _name);

    return ghost;
}
