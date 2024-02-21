// tail.c

// Gwegster @ LPUniversity
// 30-JULY-05
// A simple tail command

//Last edited on July 1st, 2005 by Tacitus

inherit STD_CMD ;

mixed main(object caller, object room, string file) {
    int morelines ;
    string *out ;

    if(!file)
        return "Usage: tail <filename>\n" ;

    file = resolve_path(caller->query("cwd"), file);

    if(directory_exists(file))
        return "Error [tail]: File specified is a directory.\n" ;

    if(!file_exists(file))
        return "Error [tail]: That file does not exist.\n" ;

    if(!caller->query_env("morelines"))
        caller->set_env("morelines", "20");

    morelines = to_int(caller->query_env("morelines"));
    out = explode(tail(file, morelines-2), "\n");
    out = ({ "File: " + file }) + out;

    return out ;
}

string help(object caller) {
    return (" SYNTAX: tail <filename>\n\n" +
    "This command prints the last 10 lines of a specified file.\n");
 }
