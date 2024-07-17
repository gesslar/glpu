//rm.c

//Tacitus @ LPUniversity
//07-APR-05
//File system management

//Last edited on August 18th, 2006 by Parthenon

inherit STD_CMD ;

string *dir_tree;
string dir;

void start_delete();
void handle_delete(string contents);

mixed main(object tp, string str) {
    dir_tree = ({});

    if(!str)
       return _info("Syntax: rm [-r] <file name>");

    if(sscanf(str, "-r %s", dir) == 1) {
        dir = resolve_path(tp->query("cwd"), dir) + "/";

        if(!directory_exists(dir))
            return _error("%s: No such directory.", dir);

        if(!(int)master()->valid_write(dir, tp, "rmdir")) {
            return _error("Permission denied.");
            return 1;
        }

        dir_tree += ({ dir });

        write("Are you sure you about that? ");
        input_to("confirm_recursive_delete", tp) ;

        return 1;
    }

    // Now check if there are any glob patterns
    if(of("*", str) || of("?", str)) {
        string *files ;
        string *failed = ({});

        files = get_dir(resolve_path(tp->query("cwd"), str));
        files -= ({ ".", ".." });

        if(!sizeof(files))
            return _error("%s: No matching file(s).", str);

        files = map(files, (: resolve_path($(tp)->query("cwd"), $1) :));

        foreach(string file in files) {
            if(!(int)master()->valid_write(file, tp, "rm")) {
                _error("Permission denied for %s.", file);
                failed += ({ file });
                continue;
            }

            if(!rm(file)) {
                failed += ({ file });
                _error("Could not remove %s.", file);
            } else {
                _ok("File removed: %s", file);
            }
        }

        if(sizeof(failed))
            return _error("Failed to remove the following files: %s", implode(failed, ", "));

        return _ok("All files removed successfully.");
    }

    str = resolve_path(tp->query("cwd"), str);

    if(directory_exists(str) || !file_exists(str))
        return _error("%s: No such file.", str);

    if(!(int)master()->valid_write(str, tp, "rm")) {
        return _error("Permission denied.");
    }

    return(rm(str) ? _ok("File removed.") : _error("Could not remove file."));
}

int confirm_recursive_delete(string arg, object tp) {
    if(!arg || arg == "" || member_array(lower_case(arg), ({ "y", "yes"})) == -1) {
        return _info("Deletion cancelled.");
        return;
    }

    start_delete();
}

void start_delete() {
    mixed *contents;

    do {
        contents = get_dir(dir);

        if(sizeof(contents) > 0)
            handle_delete(contents[0]);

    } while(sizeof(contents) > 0);

    rmdir(dir)
        ? _ok("Success: [rm]: All files and folders deleted successfully.")
        : _error("All files and folders could not be deleted.")
    ;
}

void handle_delete(string contents) {
    if(file_size(implode(dir_tree, "") + contents) == -2) {
        dir_tree += ({ contents + "/" });

        if(sizeof(get_dir(implode(dir_tree, ""))) == 0) {
            if(rmdir(implode(dir_tree, ""))) {
                dir_tree -= ({ contents + "/" });
                return;
            }
        } else {
            handle_delete(get_dir(implode(dir_tree, ""))[0]);
            dir_tree -= ({ contents + "/" });
            return;
        }
    } else if(file_size(implode(dir_tree, "") + contents) == -1)
        dir_tree -= ({ contents + "/" });
    else
        rm(implode(dir_tree, "") + contents);
}

string query_help(object tp) {
    return
"SYNTAX: rm <file name | -r dir name>\n\n"
"This command permanantly removes a file. Please note that there is no "
"'Recycle Bin'. You must be extra careful when dealing with important files."
"You may also use the -r option to recursively remove all files and folders "
"within <dir name>. It will ask you for a confirmation just to be safe." ;
}
