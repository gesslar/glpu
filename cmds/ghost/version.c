/* version.c

 Tacitus @ LPUniversity
 05-MAY-06
 Version command

*/

inherit STD_CMD ;

mixed main(object tp, string args) {
     tell(tp, mud_name() + " is running " + lib_name() + " " + lib_version() + " (" + baselib_name() + " " + baselib_version() + ")\n");
     tell(tp, "on " + driver_version() + " on " + arch() +".\n");

     return 1;
}

string help(object caller) {
     return(
" SYNTAX: version\n\n"
"This command returns version information about the software running this mud.");
}
