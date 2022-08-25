/* mudlib.h

 Tacitus @ LPUniversity
 19-OCT-05
 LPUniversity Macro File

*/

/* Last updated: August 10th, 2006 by Parthenon */

/* Redefinition warnings protection */

#ifndef MUDLIB
#define MUDLIB

#include <modules.h>

/* Standard Inheritables */

#define CMD         "/std/cmd/cmd"
#define DAEMON      "/std/daemon/daemon"
#define OBJECT      "/std/object/object"
#define OB_E        "/std/object/ob_prop"
#define ROOM        "/std/area/rooms/room"
#define MOBILE      "/std/user/mobile"
#define NPC         "/std/area/mobiles/npc"
#define MONSTER     "/std/area/mobiles/npc"

/* Mudlib Objects */

#define OBJ_PAGER          "/obj/mudlib/pager/pager"
#define OBJ_SOCKET         "/obj/mudlib/sockets/socket_ob"
#define OBJ_NEWS_STDCLIENT "/obj/mudlib/newsclients/std_newsclient"
#define OBJ_SOUL_TOOL      "/obj/mudlib/emote_admin/emote_client"
#define OBJ_MAIL_CLIENT    "/obj/mudlib/mail_clients/mail_client"

#define ANNOUNCE_CHDMOD    "/adm/daemons/chmodules/announceNet"

/* Daemons */

#define CHAN_D        "/adm/daemons/chan_d"
#define SHUTDOWN_D    "/adm/daemons/shutdown_d"
#define GA_SERVER     "/adm/daemons/ga_server"
#define NEWS_D        "/adm/daemons/news_d"
#define FINGER_D      "/adm/daemons/finger_d"
#define ANSI_PARSER   "/adm/daemons/ansi_parser"
#define XTERM256      "/adm/daemons/xterm256_d"
#define SOUL_D        "/adm/daemons/soul_d"
#define LOCKDOWN_D    "/adm/daemons/lockdown_d"
#define MAIL_D        "/adm/daemons/mail_d"

/* Redefinition warnings protection */

#endif


