inherit STD_CMD ;

mixed main(object tp, object room, string str) {
    set_debug_level("sockets") ;
    clear_debug_level("sockets") ;

    write(read_file("/log/catch")) ;
    tp->force_me("rm /log/runtime") ;
    write(read_file("/log/runtime")) ;
    tp->force_me("rm /log/catch") ;
    // tp->force_me("update -r /std/modules/http.c") ;
    // tp->force_me("update -R /adm/daemons/test_server.c");
    // tp->force_me("update -R /std/daemon/http_client") ;

    "/std/daemon/http_client"->http_request("http://discord.com/api/gateway", "GET") ;

    clear_debug_level("sockets") ;

    return 1 ;
}
