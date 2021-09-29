/* mudlib.h

 Tacitus @ LPUniversity
 19-OCT-05
 LPUniversity Macro File

*/

/* Last updated: August 10th, 2006 by Parthenon */

/* Redefinition warnings protection */

#ifndef MUDLIB
#define MUDLIB

/* Standard Inheritables */

#define OBJECT     "/std/object/object"
#define OB_E       "/std/object/ob_prop"
#define ROOM       "/std/area/rooms/room"
#define MOBILE     "/std/user/mobile"
#define NPC        "/std/area/mobiles/npc"
#define MONSTER    "/std/area/mobiles/npc"

/* Mudlib Objects */

#define OBJ_PAGER          "/obj/mudlib/pager/pager.c"
#define OBJ_SOCKET         "/obj/mudlib/sockets/socket_ob.c"
#define OBJ_NEWS_STDCLIENT "/obj/mudlib/newsclients/std_newsclient.c"
#define OBJ_SOUL_TOOL      "/obj/mudlib/emote_admin/emote_client.c"
#define OBJ_MAIL_CLIENT    "/obj/mudlib/mail_clients/mail_client.c"

#define ANNOUNCE_CHDMOD    "/adm/daemons/chmodules/announceNet.c"

/* Daemons */

#define CHAN_D        "/adm/daemons/chan_d.c"
#define SHUTDOWN_D    "/adm/daemons/shutdown_d.c"
#define GA_SERVER     "/adm/daemons/ga_server.c"
#define NEWS_D        "/adm/daemons/news_d.c"
#define FINGER_D      "/adm/daemons/finger_d.c"
#define ANSI_PARSER   "/adm/daemons/ansi_parser.c"
#define SOUL_D        "/adm/daemons/soul_d.c"
#define LOCKDOWN_D    "/adm/daemons/lockdown_d.c"
#define MAIL_D        "/adm/daemons/mail_d.c"

/* Redefinition warnings protection */

#endif


