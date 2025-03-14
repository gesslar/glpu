//quit.c

//Tacitus @ LPUniversity
//06-APR-05
//User command

//Last edited on October 8th, 2005 by Tacitus

#include <logs.h>

inherit STD_CMD;

mixed main(object caller, string arg) {
     previous_object()->exit_world();
     write("Thank you for visiting " + mud_name() + "\n");
     previous_object()->save_body();
     log_file(LOG_LOGIN, capitalize(previous_object()->query_real_name()) + " logged out from " +
     query_ip_number(previous_object()) + " on " + ctime(time()) + "\n");
     destruct(previous_object());
     return 1;
}

string help(object caller) {
     return(" SYNTAX: quit\n\n" +
     "This command will save your characher and disconnect you from the mud.\n\n" +
     "See also: save\n");
}
