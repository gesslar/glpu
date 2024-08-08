/* ln.c

 Tacitus @ LPUniversity
 15-JAN-06
 File System Command

*/

inherit STD_CMD ;

void setup() {
    usage_text = "ln <original file> <new reference>" ;
    help_text =
    "This command is a very powerful tool that allows you to create a "
    "file that references to another file. This means when you access "
    "the new file you created with this command, you'll be actually "
    "accessing the original file. This is a very dangerous tool as "
    "well because the new file will inherit the permissions of its "
    "new directory, possibly allowing people to access the file who "
    "wouldn't be able to access it before. For this command to work, "
    "you must have read and linking permissions in the directory of "
    "the original file and then write and linking permissions in the "
    "directory of the new reference." ;
}

mixed main(object caller, string args) {
    string file, reference;

    if(!args)
        return(notify_fail("Syntax: ln <original file> <new reference>\n"));

    if(sscanf(args, "%s %s", file, reference) != 2)
        return(notify_fail("Syntax: ln <original file> <new reference>\n"));

    file = resolve_path(caller->query_env("cwd"), file);
    reference = resolve_path(caller->query_env("cwd"), reference);

    if(!file_exists(file)) return(notify_fail("Error [ln]: File '" + file + "' does not exist.\n"));
    if(file_exists(reference)) return(notify_fail("Error [ln]: File '" + reference + "' already exists.\n"));
    if(directory_exists(file)) return(notify_fail("Error [ln]: '" + file + "' is a directory.\n"));
    if(directory_exists(reference)) return(notify_fail("Error [ln]: '" + reference + "' is a directory.\n"));
    if(!master()->valid_link(file, reference)) return(notify_fail("Error [ln]: Permission denied.\n"));

    write((link(file, reference) ? "Error [ln]: Linking of file '" + reference + "' to '" + file + "' was unsuccesful.\n"
        : "Success [ln]: File '" + reference + "' now linked to '" + file + "'.\n"));

    return 1;
}
