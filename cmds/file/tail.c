/**
 * @file /cmds/file/tail.c
 * @description Command to print the last 10 lines of a file.
 *
 * @created 2024-08-16 - Gesslar
 * @last_modified 2024-08-16 - Gesslar
 *
 * @history
 * 2024-08-16 - Gesslar - Created
 */

inherit STD_CMD;

void setup() {
    usage_text = "tail <file>";
    help_text = "This command prints the last 20 lines of a specified file.\n";
}

mixed main(object tp, string file) {
    string *out;

    if(!file)
        return _usage(tp);

    file = resolve_file(tp, file);

    if(!file_exists(file))
        return _error("File does not exist: %s", file);

    out = explode(tail(file, 20), "\n");
    out = ({ "=== " + file + " ===" }) + out;

    return out;
}
