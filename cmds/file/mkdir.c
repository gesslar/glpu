/**
 * @file /cmds/file/mkdir.c
 * @description Command to create a directory.
 *
 * @created 2024-08-16 - Gesslar
 * @last_modified 2024-08-16 - Gesslar
 *
 * @history
 * 2024-08-16 - Gesslar - Created
 */

inherit STD_CMD ;

void setup() {
    usage_text = "mkdir <directory name>" ;
    help_text =
"This command creates a new directory. If you specify a path, the directory "
"will be created in the specified location; otherwise, it will be created "
"in the current working directory." ;
}

mixed main(object tp, string str) {
    int result ;

    if(!str)
        return _usage(tp) ;

    str = resolve_path(tp->query_env("cwd"), str) ;

    if(file_exists(str))
        return _error("%s already a file.\n", str) ;

    if(directory_exists(str))
        return _error("%s already a directory.\n", str) ;

    if(!master()->valid_write(str, tp, "mkdir"))
        return _error("Permission denied.\n") ;

    result = mkdir(str) ;

    if(result)
        return _ok("Directory created: %s", str) ;
    else
        return _error("Could not create directory: %s", str) ;
}
