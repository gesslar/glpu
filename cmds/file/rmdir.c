/**
 * @file /cmds/file/rmdir.c
 * @description Command to remove a directory.
 *
 * @created 2024-08-16 - Gesslar
 * @last_modified 2024-08-16 - Gesslar
 *
 * @history
 * 2024-08-16 - Gesslar - Created
 */

inherit STD_CMD;

void setup() {
    usage_text = "rmdir <directory name>";
    help_text =
"This command permanantly removes a specified directory. It will not "
"currently delete a directory that has content (ie. The directory "
"you wish to remove must be empty before it can be deleted.)";
}

mixed main(object tp, string str) {
    if(!str)
        return _usage(tp);

    str = resolve_path(tp->query_env("cwd"), str);

    if(!directory_exists(str) || file_exists(str))
        return _error("%s is not a directory.", str);

    if(sizeof(get_dir(str + "/")))
        return _error("%s is not empty.", str);

    if(!master()->valid_write(str, tp, "rmdir"))
        return _error("Permission denied.");

    rmdir(str)
        ? _ok("Directory removed: %s", str)
        : _error("Could not remove directory: %s", str);

    return 1;
}
