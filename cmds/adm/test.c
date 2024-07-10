inherit STD_CMD ;

mixed main(object tp, object room, string str) {
    tp->force_me("rm /log/runtime") ;
    tp->force_me("rm /log/catch") ;

    // set_debug_level("sockets") ;
    clear_debug_level("sockets") ;

    debug(repeat_string("\n", 20)) ;
    tp->force_me("update -R /adm/daemons/discord/chatter") ;

    // clear_debug_level("sockets") ;

    return 1 ;
}
