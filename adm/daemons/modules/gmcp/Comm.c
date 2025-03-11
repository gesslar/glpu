/**
 * @file /adm/daemons/modules/gmcp/Comm.c
 * @description GMCP module to handle Comm.* packages
 *
 * @created 2024-10-10 - Gesslar
 * @last_modified 2024-10-10 - Gesslar
 *
 * @history
 * 2024-10-10 - Gesslar - Created
 */


// /adm/daemons/gmcp_modules/Core.c
//
// Created:     2024/02/23: Gesslar
// Last Change: 2024/02/23: Gesslar
//
// 2024/02/23: Gesslar - Created

#include <gmcp_defines.h>

inherit STD_DAEMON ;

varargs void Channel(object who, string sub, mapping data) {
  switch(sub) {
    case "Text":
      _debug("Text", data) ;
      assert((: mapp($(data)) && !nullp($(data)["channel"]) && !nullp($(data)["text"]) :)) ;

      if(nullp(data["talker"]))
        data["talker"] = "system" ;

      who->do_gmcp(GMCP_PKG_COMM_CHANNEL_TEXT, data) ;
      break ;
  }
}
