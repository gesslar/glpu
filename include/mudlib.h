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
#define STD_BODY            DIR_STD "user/body"
#define STD_CMD             DIR_STD "cmd/cmd"
#define STD_DAEMON          DIR_STD "daemon/daemon"
#define STD_USER            DIR_STD "user/user"
#define STD_MONSTER         DIR_STD "area/mobiles/npc"
#define STD_NPC             DIR_STD "area/mobiles/npc"
#define STD_OB_E            DIR_STD "object/ob_prop"
#define STD_OBJECT          DIR_STD "object/object"
#define STD_ROOM            DIR_STD "area/rooms/room"
#define STD_SKILL           DIR_STD "cmd/skill"
#define STD_SPELL           DIR_STD "cmd/spell"
#define STD_VIRTUAL_SERVER  DIR_STD "daemon/virtual_server"
#define STD_ZONE            DIR_STD "area/zones/zone"

/* Mudlib Objects */

#define ANNOUNCE_CHDMOD    "/adm/daemons/chmodules/announce"

#endif // __MUDLIB_H__
