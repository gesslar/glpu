/**
 * @file /std/area/virtual/zone_master.c
 * @description Master object for zone daemons
 *
 * @created 2024/07/22 - Gesslar
 * @last_modified 2024/07/22 - Gesslar
 *
 * @history
 * 2024/07/22 - Gesslar - Created
 */

inherit STD_DAEMON ;
inherit M_LOG ;

protected object generate_object(string path);

object compile_object(string path) {
    string subzone, subzone_path;
    object subzone_daemon;
    string *current_dir;
    string *path_parts;

    path_parts = explode(path, "/");
    if (sizeof(path_parts) < 1) return 0;

    current_dir = dir_file(this_object());
    subzone = path_parts[0];
    subzone_path = current_dir[0] + subzone + "/zone";

    if (file_size(subzone_path + ".c") > 0) {
        subzone_daemon = load_object(subzone_path);
        if (!subzone_daemon) {
            _log(1, "Failed to load subzone daemon: %s", subzone_path);
            return 0;
        }
        return subzone_daemon->compile_object(implode(path_parts[1..], "/"));
    }

    // No subzone found, handle the request
    return generate_object(path);
}

// To be overridden by specific zone daemons
protected object generate_object(string path) {
    _log(2, "Default generate_object called for %s", path);
    return 0;  // Default behavior: don't generate anything
}
