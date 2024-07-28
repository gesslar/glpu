/* mudlib.h

 Tacitus @ LPUniversity
 19-OCT-05
 LPUniversity Macro File

*/

/* Last updated: August 10th, 2006 by Parthenon */

/* Redefinition warnings protection */

#ifndef __MUDLIB_H__
#define __MUDLIB_H__

#include <dirs.h>

/* Standard Inheritables */

#define STD_ACT             DIR_STD "cmd/act"
#define STD_ARMOUR          DIR_STD "area/equip/armour"
#define STD_BODY            DIR_STD "user/body"
#define STD_CLOTHING        DIR_STD "area/equip/clothing"
#define STD_CMD             DIR_STD "cmd/cmd"
#define STD_CONTAINER       DIR_STD_OBJECT "container"
#define STD_CORPSE          DIR_STD_AREA "corpse/corpse"
#define STD_DAEMON          DIR_STD "daemon/daemon"
#define STD_DISCORD_BOT     DIR_STD "daemon/discord_bot"
#define STD_EQUIP           DIR_STD "area/equip/equip"
#define STD_HTTP_CLIENT     DIR_STD "daemon/http_client"
#define STD_HTTP_SERVER     DIR_STD "daemon/http_server"
#define STD_ITEM            DIR_STD "object/item"
#define STD_MONSTER         DIR_STD "area/mobiles/npc"
#define STD_NPC             DIR_STD "area/mobiles/npc"
#define STD_OB_E            DIR_STD "object/ob_prop"
#define STD_OBJECT          DIR_STD "object/object"
#define STD_REPORTER        DIR_STD "cmd/reporter"
#define STD_ROOM            DIR_STD "area/room/room"
#define STD_SKILL           DIR_STD "cmd/skill"
#define STD_SPELL           DIR_STD "cmd/spell"
#define STD_USER            DIR_STD "user/user"
#define STD_VIRTUAL_SERVER  DIR_STD "daemon/virtual_server"
#define STD_WS_CLIENT       DIR_STD "daemon/websocket_client"
#define STD_ZONE            DIR_STD "area/zones/zone"

/* Mudlib Objects */

#define ANNOUNCE_CHDMOD    "/adm/daemons/modules/channel/announce"

#endif // __MUDLIB_H__
