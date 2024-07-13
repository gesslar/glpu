/* jlo.c

   Author: Scifi@LPUniversity
   Date:   August 26, 2006
   Objective:  See the tail enf of a wizards journal
*/

inherit STD_CMD ;

#define TPN (this_player()->query_proper_name())

mixed main(object caller, string name) {
    int morelines ;
    string *out ;
    string path;

    path = "/doc/journals/journal.";

    if(name) path = path+name;
    else path = path +TPN;

    if(!file_exists(path))
        return "Error: No journal user '" + capitalize(name) + "' exists.\n";

    if(!caller->query_env("morelines"))
    caller->set_env("morelines", "20");

    morelines = to_int(caller->query_env("morelines"));
    out = explode(tail(path, morelines-2), "\n");
    out = ({ "Journal: " + path }) + out;

    return out;
}

string help(object caller)
{
    return " SYNTAX: jlo [<username>]\n\n"
       "This command will return the last ten lines \n"
       "from the wizard's journal entry.  If no argument \n"
       "is given, the current user's name will be used.\n";
}
