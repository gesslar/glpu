/**
 * @file /adm/daemons/virtual.c
 * @description Daemon for virtual objects
 *
 * @created 2024-02-04 - Gesslar
 * @last_modified 2024-07-29 - Gesslar
 *
 * @history
 * 2024-02-04 - Gesslar - Created
 * 2024-07-29 - Gesslar - Refactored to handle complex and top-level structures
 */

inherit STD_DAEMON;

public nomask object compile_object(string file) {
    string *path_components, *dir_parts;
    string vclass, module, e;
    object result;

    // Split the file path into directory and filename components
    path_components = dir_file(file);
    dir_parts = explode(path_components[0], "/") - ({""});

    // First, attempt to handle complex structures using underscore concatenation
    vclass = implode(dir_parts, "_");
    module = sprintf("%smodules/virtual/%s.c", __DIR__, vclass);

    // Check if the module exists and handle accordingly
    if(file_exists(module)) {
        e = catch(load_object(module));
        if(e) {
            log_file("VIRTUAL", e);
            return 0;
        }

        e = catch(result = call_other(module, "compile_object", file));
        if(e) {
            log_file("VIRTUAL", e);
            return 0;
        }

        return result;
    }

    // If complex structure handling fails, try the top-level directory
    if(sizeof(dir_parts) > 0) {
        vclass = dir_parts[0];
        module = sprintf("%smodules/virtual/%s.c", __DIR__, vclass);

        if(file_exists(module)) {
            e = catch(load_object(module));
            if(e) {
                log_file("VIRTUAL", e);
                return 0;
            }

            e = catch(result = call_other(module, "compile_object", file));
            if(e) {
                log_file("VIRTUAL", e);
                return 0;
            }

            return result;
        }
    }

    // Return 0 if no matching virtual daemon is found
    return 0;
}
