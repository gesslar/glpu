/**
 * @file /cmds/std/passwd.c
 * @description Command to manage your own password or the password of another
 *              user.
 *
 * @created 2024/07/14 - Gesslar
 * @last_modified 2024/07/14 - Gesslar
 *
 * @history
 * 2024/07/14 - Gesslar - Created
 */


inherit STD_CMD ;

void current_password(string str);
void new_password(string str);
void confirm_password(string str, string pass);

mixed main(object caller, string arg) {
    tell(caller, "Please enter your current password: ") ;
    input_to((:current_password:), 1, caller,);
    return 1;
}

int current_password(string str, object caller) {
    string curr = caller->query_user()->query_password();

    if(crypt(str, curr) != curr) {
        _error("The password you provided does not match your current password.");
        return ;
    } else {
        _ok("Password confirmed.\n\nPlease enter your new password: ");
        input_to((:new_password:), 1, caller);
        return ;
    }
}

void new_password(string input, object caller) {
    if(!stringp(input) || input == "") {
        _warn("Aborting password change.");
        return ;
    }

    _ok("Please enter your new password again: ");
    input_to("confirm_password", 1, caller, input);
}

void confirm_password(string input, object caller, string pass) {
    if(input != pass) {
        _error("The passwords you provided do not match.");
        return ;
    } else {
        if(caller->query_user()->set_password(input))
            _ok("Password changed successfully.");
        else
            _error("Unable to change password.");
    }
}

string query_help(object caller) {
    return
"SYNTAX: passwd\n\n"
"This command allows you to change your current passwd. You will be asked to "
"enter your current passwd for security purposes and then it will request you "
"to input your new passwd and then will ask again to confirm. After you have "
"provided the requested information, the command will inform you if the change "
"was succesful.";
}
