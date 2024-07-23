/**
 * @file /adm/daemons/modules/virtual/areas.c
 * @description Areas virtual object daemon
 *
 * @created 2024/07/22 - Gesslar
 * @last_modified 2024/07/22 - Gesslar
 *
 * @history
 * 2024/07/22 - Gesslar - Created
 */

inherit STD_DAEMON;
inherit M_LOG;

public nomask object compile_object(string file) {
    string *path_components, *dir_parts, top_level_zone, zone_path, relative_path, e;
    object result, zone_daemon;

    top_level_zone = dir_parts[1];
    zone_path = "/areas/" + top_level_zone + "/zone";

    e = catch(zone_daemon = load_object(zone_path));
    if(e) {
        log_file("VIRTUAL", e);
        return 0;
    }

    if(!zone_daemon) {
        _log(1, "Failed to load zone daemon: %s", zone_path);
        return 0;
    }

    relative_path = implode(dir_parts[2..], "/") + "/" + path_components[1];
    e = catch(result = zone_daemon->compile_object(relative_path));

    if(e) {
        log_file("VIRTUAL", e);
        return 0;
    }

    if(objectp(result))
        result->set_virtual_master(base_name(result));

    return result;
}
