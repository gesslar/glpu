/**
 * @file /areas/village/zone.c
 * @description Zone controller for the village area
 *
 * @created 2024/07/22 - Gesslar
 * @last_modified 2024/07/22 - Gesslar
 *
 * @history
 * 2024/07/22 - Gesslar - Created
 */

inherit STD_ZONE_D ;

protected varargs object generate_object(string path) {
    string vtype, dest ;

    _debug("Path = %s", path) ;

    if(sscanf(path, "%s/%s", vtype, dest) != 2)
        return 0 ;

    _debug("vtype = %s, dest = %s", vtype, dest) ;
    switch(vtype) {
        case "field":
            return new(__DIR__ "field_inherit", dest) ;
        default:
            return 0 ;
    }

}
