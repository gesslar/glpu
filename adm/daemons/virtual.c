// /adm/daemons/virtual.c
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_DAEMON ;

object compile_object(string file) {
    string *pathExploded, serverPath;
    object server, virtualObject;
    int index;

    write_file("/log/virtual_compile", "Request: " + file + " ("+previous_object()+")\n") ;

    pathExploded = explode(file, "/");
    index = member_array("virtual_area", pathExploded);

    if(index != -1) {
        serverPath = "/" + implode(pathExploded[0..index], "/") + "/virtual_server.c";
        if(file_exists(serverPath)) {
            write_file("/log/virtual_compile", "Server: " + serverPath + " ("+previous_object()+")\n") ;

            catch(server = load_object(serverPath));
            if(objectp(server)) {
                virtualObject = server->compile_object(file);

                if(objectp(virtualObject))  {
                    write_file("/log/virtual_compile", "Virtual object loaded for " + file + " ("+previous_object()+")\n") ;
                    virtualObject->set_virtual_master(base_name(virtualObject)) ;
                    return virtualObject;
                } else {
                    write_file("/log/virtual_compile", "No Virtual object loaded for " + file + " ("+previous_object()+")\n") ;
                    return 0;
                }
            }
        }

        write("No functional server found at " + serverPath + " ("+previous_object()+")\n") ;
    }

    return 0;
}
