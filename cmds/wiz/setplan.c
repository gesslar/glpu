/**
 * @file /cmds/wiz/setplan.c
 * @description Command to manage one's .plan file
 *
 * @created 2006/07/22 - Parthenon
 * @last_modified 2024/07/20 - Gesslar
 *
 * @history
 * 2006/07/22 - Parthenon - Created
 * 2006/07/23 - Parthenon - Updated - Gesslar has no idea what they updated
 * 2024/07/20 - Gesslar - Entirely rewrote the command and added support
 *                        for the new editor object.
 */

inherit STD_CMD ;

private int delete_plan(object tp);
public finish_edit(string text, object tp, int overwrite);

mixed main(object tp, string arg) {
    string tmp = "";
    string user = query_privs(tp);
    object editor ;
    int overwrite = false ;
    string file ;

    if(arg && (arg == "-d" || sscanf(arg, "-d %*s")))
        return delete_plan(tp);

    file = user_path(user) + ".plan";

    if(arg && (arg == "-o" || sscanf(arg, "-o %*s"))) {
        overwrite = true;
    } else {
        overwrite = false;
    }

    if(overwrite)
        tell(tp, "Enter the text you wish to have for your plan.\n") ;
    else
        tell(tp, "Enter the text you wish to append to your plan.\n") ;

    _debug("file: %s", file) ;
    _debug("overwrite: %d", overwrite) ;

    editor = new(OBJ_EDITOR) ;
    if(overwrite) {
        editor->edit(tp, null, assemble_call_back((:finish_edit:), tp, file, overwrite)) ;
    } else {
        if(file_exists(file))
            editor->edit(tp, file, assemble_call_back((:finish_edit:), tp, file, overwrite)) ;
        else
            editor->edit(tp, null, assemble_call_back((:finish_edit:), tp, file, overwrite)) ;
    }

    return 1;
}

public void finish_edit(string text, object tp, string file, int overwrite) {
    if(!text || text == "")
        return tell(tp, "Aborted.\n") ;

    if(!write_file(file, text, overwrite)) {
        _error(tp, "Plan could not be written.\n") ;
        return;
    }

    _ok(tp, "Plan successfully written to %s", file) ;
}

int delete_plan(object tp) {
    string name = query_privs(tp);
    string file = user_path(name) + ".plan";

    if(!file_exists(file))
        return _error("No such file %s", file);

    if(!rm(file))
        return _error("Could not delete plan file %s", file);

    return _ok("Plan file %s deleted", file);
}

string query_help(object tp) {
    return
"SYNTAX: setplan [ -o|-d ]\n\n"
"This command allows you to edit your .plan that is shown in your finger "
"information. If you use the '-o' option then your plan will be overwritten "
"otherwise what you type in will be appended to your current .plan.\n\n"
"You may also delete your plan by using the '-d' option.\n";
}
