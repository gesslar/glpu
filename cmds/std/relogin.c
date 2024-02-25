/* relogin.c

 Tacitus @ LPUniversity
 12-JUNE-06
 Relogin command

*/

inherit STD_CMD ;

mixed main(object tp, object room, string args) {
    LOGIN_OB->relogin();
    return 1;
}
