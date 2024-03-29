//ed.c

//Byre @ LPUniversity
//05-APR-05
//File editor

//Last edited on October 6th, 2006 by Tacitus

#include <logs.h>

inherit STD_CMD ;

int main(object caller, object room, string file)
{
    if(!file)
    {
        if(!caller->query("cwf"))
            return notify_fail("Error [ed]: No argument supplied.\n");
        file = caller->query("cwf");
    }

    file=resolve_path(caller->query("cwd"),file);
    caller->set("cwf", file);

    if(directory_exists(file))
    return notify_fail("Error: Can not edit a directory.\n");

    write(file + "\n");
    ed(file);
    log_file(LOG_ED, capitalize(query_privs(caller)) + " edited " + file
        + " on " + ctime(time()) + "\n");
    return 1;
}
string help(object caller) {
    return (" SYNTAX: ed <file>\n\n" +
    "This command lets you edit a specified file in the mud editor. To\n"
    "access editor specific help, type 'h' while in the editor.\n\n" +
    " See also: rm, more, mv, cp, etc.\n");
}
