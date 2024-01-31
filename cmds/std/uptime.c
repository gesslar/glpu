//uptime.c

//Tacitus @ LPUniversity
//08-APR-05
//Standard command

//Last Edited on October 20th, 2005 by Tacitus

inherit STD_CMD ;

int main(object caller, object room, string arg) {
     write(capitalize(mud_name()) + " has been running since " + ctime(time() - uptime()) + "\n");
     return 1;
}

string help(object caller)
{
     return(" SYNTAX: uptime\n\n" +
     "This command will tell you how the long them mud has been running.\n");
}
