// /adm/daemons/gmcp_modules/Char.c
//
// Created:     2024/02/23: Gesslar
// Last Change: 2024/02/23: Gesslar
//
// 2024/02/23: Gesslar - Created

#include <gmcp.h>

inherit STD_DAEMON ;

void Status(object who) {
    mapping status ;

    status = ([
        GMCP_LBL_CHAR_STATUS_NAME        : who->query_name(),
        GMCP_LBL_CHAR_STATUS_CAP_NAME    : who->query_cap_name(),
        GMCP_LBL_CHAR_STATUS_CAPACITY    : who->query_capacity(),
        GMCP_LBL_CHAR_STATUS_MAX_CAPACITY: who->query_max_capacity(),
    ]) ;

    who->do_gmcp(GMCP_PKG_CHAR_STATUS, status) ;
}
