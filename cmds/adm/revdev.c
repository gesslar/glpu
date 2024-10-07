/* revdev.c

 Tacitus @ LPUniversity
 15-JAN-06
 Admin command

*/

//Last edited on October 6th, 2006 by Tacitus

#include <logs.h>

inherit STD_CMD ;

mixed main(object caller, string args) {
    object body ;
    object security_editor ;

    if(!adminp(previous_object()))
        return _error("Access denied.") ;
     if(!args)
        return(_error("revdev <user>")) ;

    args = lower_case(args) ;
    body = find_player(args) ;

    if(!body)
        return _error("User '%s' not online.", args) ;

    if(!devp(query_privs(body)))
        return _error("That user is not a developer.") ;

    _info("Revoking developer access for '%s'.", capitalize(body->query_real_name())) ;

    _ok(body, "Developer Access Revoked.") ;
    body->rem_path("/cmds/wiz/") ;
    body->rem_path("/cmds/object/") ;
    body->rem_path("/cmds/file/") ;
    body->rem_path("/cmds/adm/") ;
    security_editor = new(OBJ_SECURITY_EDITOR) ;
    security_editor->disable_membership(query_privs(body), "developer") ;
    security_editor->write_state(0) ;
    security_editor->remove() ;
    body->save_body() ;

    _ok("User '%s' is no longer a developer.", capitalize(body->query_real_name())) ;

    log_file(LOG_PROMOTE, capitalize(query_privs(caller)) + " revoked "
        + body->query_real_name() + "'s developer status on " + ctime(time())
        + "\n") ;
    return 1 ;
}

string help(object caller) {
    return(" SYNTAX: revdev <user>\n\n"
    "This command will revoke a user's developer access to\n"
    "the mud by removing them from the group 'developer' and\n"
    "by removing the directories '/cmds/wiz/', '/cmds/object/'\n"
    "and '/cmds/file/' from the user's command path.\n\n"
    "See also: makedev\n") ;
}
