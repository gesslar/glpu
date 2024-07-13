//passwd.c

//Tacitus @ LPUniversity
//17-OCT-05
//Standard command

/* Last edited on July 17th, 2006 by Tacitus */

int confirm_CurrentPassword(string str);
int i_NewPassword(string str);
int confirm_NewPassword(string str, string pass);

inherit STD_CMD ;


mixed main(object caller, string arg) {
    tell(caller, "Please enter your current password: ", MSG_PROMPT) ;
    input_to("confirm_CurrentPassword", 1, caller,);
    return 1;
}

int confirm_CurrentPassword(string str, object caller) {
    string curr = caller->query_user()->query_password();
    if(crypt(str, curr) != curr) {
        tell(caller, "\nError: The password you provided does not match your current password.\n");
        return 1;
    } else {
        tell(caller, "\nPlease enter your new password: ", MSG_PROMPT);
        input_to("i_NewPassword", 1, caller);
        return 1;
    }
}

int i_NewPassword(string str, object caller) {
    if(!stringp(str) || str == "") {
        tell(caller, "\n\nError [passwd]: Invalid password.\n");
        return 1;
    }

    tell(caller, "\nPlease enter your new password again: ", MSG_PROMPT);
    input_to("confirm_NewPassword", 1, caller, str);
    return 1;
}

int confirm_NewPassword(string str, object caller, string pass) {
    if(str != pass) {
        tell(caller, "\nError: Passwords do not match. Please try again.\n");
        return 1;
    } else {
        if(caller->query_user()->set_password(str))
            tell(caller, "\nSuccess: Password changed successfully.\n");
        else
            tell(caller, "\nError: Unable to change password.\n");
    }
    return 1;
}

string help(object caller) {
    return(
" SYNTAX: passwd\n\n"
"This command allows you to change your current passwd. You will be asked to "
"enter your current passwd for security purposes and then it will request you "
"to input your new passwd and then will ask again to confirm. After you have "
"provided the requested information, the command will inform you if the change "
"was succesful.");
}
