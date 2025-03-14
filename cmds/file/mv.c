/**
 * @file /cmds/file/mv.c
 * @description Move a file or directory
 *
 * @created 2024-08-16 - Gesslar
 * @last_modified 2024-08-16 - Gesslar
 *
 * @history
 * 2024-08-16 - Gesslar - Created
 */

inherit STD_CMD;

void setup() {
    usage_text = "mv <origin> <dest>";
    help_text =
"Moves a file or directory from origin to destination. The origin and "
"destination can be either absolute or relative paths.\n"
"* If the destination is a directory, the origin will be moved inside the directory.\n"
"* If the destination does not exist, it will be created.\n"
"* If the source does not exist, an error will be returned.\n"
"* If the source is a directory, the destination must be a directory as well.\n"
"* If the destination is a file, an error will be returned.";
}

mixed main(object tp, string str) {
    string origin, dest;
    int result;

    if(!str || !sscanf(str, "%s %s", origin, dest))
        return _usage(tp);

    origin = resolve_path(tp->query_env("cwd"), origin);
    dest = resolve_path(tp->query_env("cwd"), dest);

    if(!master()->valid_write(origin, tp, "mv") || !master()->valid_write(dest, tp, "mv"))
        return _error(tp, "Permission denied.");

    result = rename(origin, dest);

    if(result < 0)
        return _error(tp, "Move failed.");

    return _ok(tp, "Moved %s to %s", origin, dest);
}
