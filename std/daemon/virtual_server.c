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
    file = implode(parts[index+1..], "/") ;

    parts = explode(file, "?") ;

    if(sizeof(parts) == 1) {
        result["base"] = UNDEFINED ;
        file = parts[0] ;
    } else {
        result["base"] = parts[0] ;
        file = parts[1] ;
    }

    parts = explode(file, "&") ;
    sz = sizeof(parts) ;
    while(sz--) {
        string *pair = explode(parts[sz], "=") ;
        result[pair[0]] = pair[1] ;
    }

    return result ;
}
