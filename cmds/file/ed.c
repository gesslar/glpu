//ed.c

//Byre @ LPUniversity
//05-APR-05
//File editor

//Last edited on October 6th, 2006 by Tacitus

#include <logs.h>
#include <ed.h>

inherit STD_CMD;

void ed_call_back(int status, string file, string temp_file, object tp);

mixed main(object tp, string file) {
    string e;
    string *parts;

    if(!file) {
        if(!tp->query_env("cwf"))
            return _error("No argument supplied.");
        file = tp->query_env("cwf");
    }

    file = resolve_path(tp->query_env("cwd"), file);

    if(directory_exists(file))
        return _error("Cannot edit a directory.");

    parts = dir_file(file);
    if(!directory_exists(parts[0]))
        return _error("No such directory: " + parts[0]);

    tp->set_env("cwf", file);
    _info("Editing %s", file);

    tp->ed_edit(file);

    log_file(LOG_ED, capitalize(query_privs(tp)) + " edited " + file
        + " on " + ctime(time()) + "\n");
    return 1;
}

string help(object tp) {
    return (" SYNTAX: ed <file>\n\n" +
    "This command lets you edit a specified file in the mud editor. To\n"
    "access editor specific help, type 'h' while in the editor.\n\n" +
    " See also: rm, more, mv, cp, etc.\n");
}
