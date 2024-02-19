// /adm/daemons/virtual.c
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_DAEMON ;

object compile_object(string file) {
    string *path_exploded, server_path;
    object server, virtual_object;
    int index;

    write_file("/log/virtual_compile", "Request: " + file + " ("+previous_object()+")\n") ;

    path_exploded = explode(file, "/");
    index = member_array("virtual_area", path_exploded);

    if(index != -1) {
        server_path = "/" + implode(path_exploded[0..index], "/") + "/virtual_server.c";
        if(file_exists(server_path)) {
            write_file("/log/virtual_compile", "Server: " + server_path + " ("+previous_object()+")\n") ;

            catch(server = load_object(server_path));
            if(objectp(server)) {
                virtual_object = server->compile_object(file);

                if(objectp(virtual_object))  {
                    write_file("/log/virtual_compile", "Virtual object loaded for " + file + " ("+previous_object()+")\n") ;
                    virtual_object->set_virtual_master(base_name(virtual_object)) ;
                    return virtual_object;
                } else {
                    write_file("/log/virtual_compile", "No Virtual object loaded for " + file + " ("+previous_object()+")\n") ;
                    return 0;
                }
            }
        }

        write("No functional server found at " + server_path + " ("+previous_object()+")\n") ;
    }

    return 0;
}
