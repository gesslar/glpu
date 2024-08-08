//mv.c

//Tacitus @ LPUniversity
//08-APR-05
//File system management

//Last edited on July 1st, 2005 by Tacitus

inherit STD_CMD ;

mixed main(object caller, string str) {
    string source, dest;
    if(!str || !sscanf(str, "%s %s", source, dest))
        return notify_fail("Syntax: mv <source> <dest>\n");
    source = resolve_path(caller->query_env("cwd"), source);
    dest = resolve_path(caller->query_env("cwd"), dest);
    if(!(int)master()->valid_write(source, caller, "mv") || !(int)master()->valid_write(dest, caller, "mv")) {
        write("Error [mv]: Permission denied.\n");
        return 1;
    }

    if(rename(source, dest) < 0) {
        write("Error [mv]: Move failed.\n");
    } else {
        write("Successful [mv]: " + source + " moved to " + dest + "\n");
    }
    return 1;
}
string help(object caller) {
    return (" SYNTAX: mv <source> <destination>\n\n" +
    "This command moves a file to a destination you specify. The first argument\n"
    "is the file you wish to move and the second argument is the destination.\n");
}
