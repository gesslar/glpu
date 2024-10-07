/* nuke.c

Tacitus @ LPUniversity
05-MAY-06
Standard nuke command V2

*/

/* Last edited on October 6th, 2006 by Tacitus */

#include <logs.h>

inherit STD_CMD ;

mixed main(object tp, string user) {
    if(!adminp(previous_object()))
        return _error("Access denied.") ;

    if(!user)
        return _info("Usage: nuke <player>") ;

    user = lower_case(user) ;

    if(!user_exists(user))
        return _error("User '%s' does not exist.", user) ;

    _question(tp, "Are you sure you want to delete " + user + "? [y/n] ", MSG_PROMPT) ;
    input_to("confirm_nuke", 0, tp, user) ;

    return 1 ;
}

void confirm_nuke(string str, object tp, string user) {
    object security_editor ;
    object body, link ;
    string *dir ;

    if(str != "y" && str != "yes") {
        _info(tp, "Abort [nuke]: Aborting nuke.") ;
        return ;
    }

    _info(tp, "Stripping user of system group memberships.") ;

    security_editor = new(OBJ_SECURITY_EDITOR) ;

    foreach(mixed group in security_editor->list_groups()) {
        if(is_member(user, group))
            _info(tp, "* Removing from group: %s.", group) ;
        security_editor->disable_membership(user, group) ;
    }

    security_editor->write_state(0) ;
    security_editor->remove() ;

    if(body = find_player(user)) {
        _info(tp, "Disconnecting user '" + user + "'.") ;
        _ok(body, "You watch as your body dematerializes.") ;

        if(environment(body)) {
            tell_down(environment(body), "You watch as " + capitalize(user) + " dematerializes before your eyes.\n",
                0, ({ body })) ;
            link = body->query_user() ;
            link->remove() ;
            body->remove() ;
        }
    }

    _info(tp, "Deleting pfile for user '%s'.", capitalize(user)) ;

    dir = get_dir(user_data_directory(user)) ;
    foreach(string file in dir) {
        _info(tp, "* Deleting file: %s.", file) ;
        if(!rm(user_data_directory(user) + file)) {
            _error(tp, "Error while deleting %s in user directory.", file) ;
            return ;
        }
    }
    rmdir(user_data_directory(user)) ;

    _ok(tp, "User '%s' has been removed.", capitalize(user)) ;
    log_file(LOG_NUKE, capitalize(query_privs(tp)) + " nukes " + capitalize(user) + " on " + ctime(time()) + "\n") ;
}

string help(object tp) {
    return (
" SYNTAX: nuke <username>\n\n" +
"This command will delete the target user's pfile, thus removing their account "
"from " + mud_name() + ". Furthermore it will also remove their membership "
"from all groups. This is NON REVERSABLE, so use with discretion.\n\n"
" See also: lockdown") ;
}
