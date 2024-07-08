/**
 * @file /adm/daemons/modules/gmcp/Core.c
 * @description GMCP module to handle Core.* packages
 *
 * @created 2024/07/08 - Gesslar
 * @last_modified 2024/07/08 - Gesslar
 *
 * @history
 * 2024/07/08 - Gesslar - Created
 */


// /adm/daemons/gmcp_modules/Core.c
//
// Created:     2024/02/23: Gesslar
// Last Change: 2024/02/23: Gesslar
//
// 2024/02/23: Gesslar - Created

#include <gmcp.h>

inherit STD_DAEMON ;

void Ping(object who) {
    who->do_gmcp(GMCP_PKG_CORE_PING) ;
}
