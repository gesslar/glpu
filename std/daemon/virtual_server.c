// /std/daemon/virtual_server.c
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_DAEMON ;

protected mapping virtual_info(string file) {
    mapping result = ([ ]) ;
    string *parts ;
    int index, sz ;

    parts = explode(file, "/") ;
    index = member_array("virtual_area", parts) ;
    parts = parts[index+1..] ;
debug_message(sprintf("parts: %O\n", parts)) ;

    if(sizeof(parts) < 1)
        return result ;

    result["base"] = parts[0] ;
    result["file"] = parts[1..] ;

    return result ;
}
