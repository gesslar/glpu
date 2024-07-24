//update.c

//Byre @ LPUniversity
//05-APR-05
//Object management

//Last edited July 11th, 2006 by Tacitus
// Last Change: 2024/02/04: Gesslar
// - Added recursion to the update command

inherit STD_CMD ;

void do_update(string file) ;
string *collect_inherits(object obj, int depth) ;
string *deep_collect_inherits(object obj, mapping seen, int depth) ;

mixed main(object tp, string arg) {
    object obj, room;
    string error, *users;
    string *files, file, *parts, start;
    int depth = 0;
    int is_room_obj;

    if(!objectp(room = environment(tp)))
        return "You are not in a valid environment.";

    if(arg) {
        if(arg == "-r" || arg == "-R") {
            if(arg == "-r") depth = 1;
            else if(arg == "-R") depth = 2;
        } else if(sizeof(parts = pcre_extract(arg, "^-([rR]) (.*)$")) == 2) {
            if(parts[0] == "r") depth = 1;
            else depth = 2;

            file = parts[1];
        } else {
            depth = 0;
            file = arg;
        }
    }

    if(!file) {
        if(!tp->query_env("cwf"))
            return _error("You must provide an argument. Syntax: update [-rR] [<file>]");
        file = tp->query_env("cwf");
    } else {
        // First, try to find an object with the given name
        obj = present(file, tp);
        if(!obj)
            obj = present(file, room);

        if(obj) {
            // If we found an object, check if it's a clone
            if(clonep(obj)) {
                file = base_name(obj);
            } else {
                file = file_name(obj);
            }
        }
        // If we didn't find an object, 'file' remains as the input string
    }

    // At this point, 'file' should be a file name or a file path
    file = resolve_path(tp->query_env("cwd"), file);

    // Always append ".c" to the file name if it's not already there
    file = append(file, ".c");

    // Check if we're trying to update the update command itself
    if(file == append(file_name(), ".c")) {
        return _error("You cannot update the update command. Destruct it instead.");
    }

    if(file[0..<3] == VOID_ROOM) {
        return _error("You cannot update the void object.");
    }

    // if(!file_exists(file))
    //     return _error("%s does not exist.", file);

    start = file;

    if(!obj) {
        obj = find_object(file);
    }

    is_room_obj = objectp(obj) && obj->is_room();

    if(objectp(obj)) {
        if(clonep(obj)) {
            _info(tp, "Updating master of %s", file_name(obj));
        } else {
            _info(tp, "Updating %s", file);
        }

        if(virtualp(obj)) {
            file = obj->query_virtual_master();
            file = append(file, ".c");
        }

        if(is_room_obj) {
            users = filter_array(all_inventory(obj), (: interactive :));
            users->move(load_object(VOID_ROOM), 1);
            obj->remove();
            if(obj) destruct(obj);
        }
        // For non-room objects, we don't destruct or move users
    } else {
        _info(tp, "Updating %s", file);
    }

    tp->set_env("cwf", start);

    obj = load_object(file);
    files = collect_inherits(obj, depth);
    files = map(files, (: append($1, ".c") :));
    filter(files, (: do_update :));

    obj = load_object(start);
    if(is_room_obj && pointerp(users)) users->move(obj, 1);
    return 1;
}

void do_update(string file, string vfile) {
    object obj;
    string error, *users;
    string *files ;

    if(obj = find_object(file)) {
        obj->remove() ;
        if(obj) destruct(obj);
    }

    if(!file_exists(file))
        _error("%s does not exist.", file);

    error = catch(obj = load_object(file));

    if(error){
        _error("Failed to load: %s\n%s", file, error);
        return ;
    }

    if(pointerp(users)) users->move(obj, 1);
    _ok("%s was updated.", file) ;
}

// Function to collect immediate and deep inherits.
string *collect_inherits(object obj, int depth) {
    string fname = file_name(obj); // The file name of the requested object
    mapping seen = ([ fname: 1 ]); // Initialize seen with the object itself to avoid adding it twice
    string *files = ({});

    if (depth == 0) {
        // Directly return the file name of the object for depth 0
        return ({ fname });
    } else if (depth >= 1) {
        // Collect inherits according to depth
        files = deep_collect_inherits(obj, seen, depth);
        // Ensure the object itself is included last for depths 1 and 2
        if (!sizeof(files) || files[<1] != fname) {
            files += ({ fname });
        }
    }

    // No need to reverse for correct order as we want the base classes loaded first
    return files;
}

string *deep_collect_inherits(object obj, mapping seen, int depth) {
    string *files = ({});

    if (depth > 0) {
        foreach (string file in inherit_list(obj)) {
            if (!seen[file]) {
                object inherit_obj = find_object(file) || load_object(file);
                seen[file] = 1; // Mark as seen to avoid duplicates

                // For depth 2, recursively collect inherits; add directly for depth 1
                if (depth == 2 && inherit_obj) {
                    files += deep_collect_inherits(inherit_obj, seen, depth);
                }
                files += ({ file });
            }
        }
    }

    return files;
}

string help(object tp) {
    return
"SYNTAX: update [-rR] [<file|here>]\n\n" +
"If file is not specified, the current working file will be updated. If the "
"file is specified, it will be updated. If the -r or -R flag is specified, "
"the file will be updated along with all of its inherited files. The -r flag "
"will only update the first level of inheritance, while the -R flag will "
"update all levels of inheritance.\n\n"
"Examples:\n"
"update - Update the cwf.\n"
"update here - Update the current room.\n"
"update -r here - Update the current room and its inherit_list().\n"
"update -R here - Update the current room and its deep_inherit_list().\n"
"update /cmds/std/who - Update the who command.\n"
"update -r /cmds/std/who - Update the who command and its inherit_list().\n"
"update -R /cmds/std/who - Update the who command and its deep_inherit_list().\n"
"update -r - Update the cwf and its inherit_list().\n"
"update -R - Update the cwf and its deep_inherit_list().\n" ;
}
