/* date.c

 Tacitus @ LPUniversity
 10-NOV-05
 Standard Command

*/

inherit STD_CMD;

mixed main(object caller, string str) {
     printf(" The current date is %s.\n", ctime(time()));
     return 1;
}

string help(object caller) {
     return("SYNTAX: date\n\n"
     "This command will return the current date. It is important\n"
     "to note though that the date is in mud time, not in your\n"
     "localtime.\n");
}
