/* revdev.c

 Tacitus @ LPUniversity
 15-JAN-06
 Admin command

*/

//Last edited on October 6th, 2006 by Tacitus

#include <logs.h>

inherit STD_CMD ;

mixed main(object caller, string args) {
    object user;
    object security_editor;

    if(!adminp(previous_object()))
        return notify_fail("Error [redev]: Access denied.\n");
     if(!args)
        return(notify_fail("Syntax: revdev <user>\n"));

    args = lower_case(args);
    user = find_player(args);

    if(!user)
        return(notify_fail("Error [makedev]: User '" + args + "' is not online.\n"));
    if(!devp(query_privs(user)))
        return(notify_fail("Error [makedev]: That user is not a developer.\n"));

    write("Revoking developer access for '" + capitalize(user->query_real_name()) + "'.\n");
    tell_object(user, "\nDeveloper Access Revoked.\n");
    user->rem_path("/cmds/wiz/");
    user->rem_path("/cmds/object/");
    user->rem_path("/cmds/file/");
    user->rem_path("/cmds/adm/");
    security_editor = new(OBJ_SECURITY_EDITOR);
    security_editor->disable_membership(query_privs(user), "developer");
    security_editor->write_state(0);
    security_editor->remove() ;
    user->save_user();

    write("Success [revdev]: User '" + capitalize(user->query_real_name()) + "' is no longer a developer.\n");
    log_file(LOG_PROMOTE, capitalize(query_privs(caller)) + " revoked "
        + user->query_real_name() + "'s developer status on " + ctime(time())
        + "\n");
    return 1;
}

string help(object caller) {
    return(" SYNTAX: revdev <user>\n\n"
    "This command will revoke a user's developer access to\n"
    "the mud by removing them from the group 'developer' and\n"
    "by removing the directories '/cmds/wiz/', '/cmds/object/'\n"
    "and '/cmds/file/' from the user's command path.\n\n"
    "See also: makedev\n");
}
