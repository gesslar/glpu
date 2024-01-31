/* version.c

 Tacitus @ LPUniversity
 05-MAY-06
 Version command

*/

inherit CMD ;

int main()
{
     write(mud_name() + " is running " + baselib_name() + " " + lib_version() + "\n");
     write("  on " + driver_version() + " on " + arch() +".\n");

     return 1;
}

string help(object caller)
{
     return(" SYNTAX: version\n\n"
     "This command returns version information about the software\n"
     "running this mud.\n");
}
