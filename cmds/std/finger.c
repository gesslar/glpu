/* finger.c

 Tacitus @ LPUniversity
 26-MAR-06
 Std Command
*/

mixed main(object caller, string user) {
    string who, where ;

    if(!user)
        return(notify_fail("Syntax: finger <user>\n")) ;

    write(FINGER_D->finger_user(user)) ;

    return 1 ;
}

string help(object caller) {
    string ret =
    " SYNTAX: finger <user>\n\n"
    "This command will display information regarding an\n"
    "existing user such as their e-mail address, last login\n"
    "rank, plan, etc. This command will be able to retrieve\n"
    "the information even if they are offline.\n" ;
    if(devp(caller)) ret += "\n See also: profile\n" ;

    return ret ;
}
