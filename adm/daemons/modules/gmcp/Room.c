#include <daemons.h>
#include <gmcp_defines.h>

void Info(object who, object room) {
    mapping data = ([ ]);

    if(!room)
        return;

    data = room->gmcp_room_info(who);

    who->do_gmcp(GMCP_PKG_ROOM_INFO, data, 1);
}

void Travel(object who, string *stops) {
    stops = map(stops, (: hash("md4", $1) :));

    who->do_gmcp(GMCP_PKG_ROOM_TRAVEL, stops, 1);
}
