/* get.c

 Ico2 @ LPUniversity
 27-OCTOBER-05
 Standard Command

QC Review: Tacitus on October 28th, 2005

Last edited on March 24th, 2006 by Tacitus

*/

inherit STD_CMD ;

int main(object caller, object room, string arg)
{
    object ob;

    if(!arg)
    return(notify_fail("SYNTAX: get <object>\n"));

    ob = present(arg, environment(caller));

    if(!ob)
    return(notify_fail("Error [get]: The object '" + arg + "' can not be found.\n"));

    if(ob->query("prevent_get") || ob->prevent_get())
    return(notify_fail("Error [get]: The object '" + arg + "' can not be picked up.\n"));

    if(!ob->move(caller))
    return(notify_fail("Error [get]: Unable to get '" + arg + "'.\n"));

    write("Success [get]: You get a '" + get_short(ob) + "'\n");
    say(capitalize(caller->name()) + " gets a '" + ob->query("short") + "'.\n");

    return 1;
}

string help(object caller)
{
    return(" SYNTAX: get <item>\n\n"
      "This command will allow you to get an object from your current\n"
      "environment. The argument you provide, will be the name of the\n"
      "object you wish to drop.\n\n" +
      "See also: drop\n");
}
