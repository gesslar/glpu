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
