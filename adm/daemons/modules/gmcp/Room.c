#include <daemons.h>
#include <gmcp_defines.h>

void Info(object who) {
    object room = environment(who) ;
    mapping data = ([ ]) ;

    if(!room)
        return ;

    data = room->gmcp_room_info(who) ;

    who->do_gmcp(GMCP_PKG_ROOM_INFO, data, 1) ;
}

void Travel(object who, string file) {
    mapping data = ([
        "destination" : hash("md4", file),
    ]) ;

    who->do_gmcp(GMCP_PKG_ROOM_TRAVEL, data, 1) ;
}
