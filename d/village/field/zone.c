// /d/village/virtual_area/virtual_server.c
// Virtual server for the Olum zone
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_VIRTUAL_SERVER ;

object generate_object(string file) {
    object result ;

    if(pcre_match(file, "^field\\d+$")) {
        result = new(__DIR__ "field", file) ;
        return result ;
    }

    return 0 ;
}
