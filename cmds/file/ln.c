/* ln.c

 Tacitus @ LPUniversity
 15-JAN-06
 File System Command

*/

inherit STD_CMD ;

void setup() {
    usage_text = "ln <original file> <new reference>" ;
    help_text =
"This command is a very powerful tool that allows you to create a file that "
"references to another file. This means when you access the new file created "
"with this command, you'll be actually accessing the original file. This is a "
"very dangerous tool as well because the new file will inherit the "
"permissions of its original file, possibly allowing people to access the "
"file who wouldn't be able to access it before. For this command to work, you "
"must have read and linking permissions in the directory of the original file "
"as well as write and linking permissions in the directory of the new "
"reference." ;
}

mixed main(object tp, string args) {
    string file, reference ;
    int result ;

    if(!args)
        return _usage(tp) ;

    if(sscanf(args, "%s %s", file, reference) != 2)
        return _usage(tp) ;

    file = resolve_path(tp->query_env("cwd"), file) ;
    reference = resolve_path(tp->query_env("cwd"), reference) ;

    if(!file_exists(file))
        return _error(tp, "File '%s' does not exist.", file) ;

    if(file_exists(reference))
        return _error(tp, "File '%s' already exists.", reference) ;

    if(directory_exists(file))
        return _error(tp, "'%s' is a directory.", file) ;

    if(directory_exists(reference))
        return _error(tp, "'%s' is a directory.", reference) ;

    if(!master()->valid_link(file, reference))
        return _error(tp, "Permission denied.") ;

    link(file, reference) < 0
        ? _error(tp, "Unable to link '%s' to '%s'.", reference, file)
        : _ok(tp, "'%s' now linked to '%s'.", reference, file) ;

    return 1 ;
}
