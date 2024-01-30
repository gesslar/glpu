//uptime.c

//Tacitus @ LPUniversity
//08-APR-05
//Standard command

//Last Edited on October 20th, 2005 by Tacitus

inherit CMD ;

int main()
{
     write(HIW + capitalize(mud_name()) + " has been running since " + NOR + HIG + ctime(time() - uptime()) + "\n" + NOR);
     return 1;
}

string help(object caller)
{
     return(HIW + " SYNTAX: " + NOR + "uptime\n\n" +
     "This command will tell you how the long them mud has been running.\n");
}
