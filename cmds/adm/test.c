inherit STD_CMD ;

mixed main(object tp, object room, string str) {
    // set_debug_level("sockets") ;
    clear_debug_level("sockets") ;

    tp->force_me("rm /log/runtime") ;
    tp->force_me("rm /log/catch") ;
    tp->force_me("update -r /std/modules/http") ;
    tp->force_me("update -r /std/daemon/http_client") ;
    tp->force_me("update -r /std/daemon/websocket_client") ;
    tp->force_me("update -r /adm/daemons/discord") ;

    // clear_debug_level("sockets") ;

    return 1 ;
}
