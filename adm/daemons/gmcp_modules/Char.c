// /adm/daemons/gmcp_modules/Char.c
//
// Created:     2024/02/23: Gesslar
// Last Change: 2024/02/23: Gesslar
//
// 2024/02/23: Gesslar - Created

#include <gmcp.h>

inherit STD_DAEMON ;

// This function should present all of the labels that correspond to the
// variables that are sent in the Char.Status package. The purpose of this
// is to inform the client what to display, if desired, for each variable
// on labels in a GUI or messaging resulting from GMCP.
//
// The labels and values are all found in /include/gmcp.h
void StatusVars(object who) {
    mapping data ;

    data = ([
        GMCP_LBL_CHAR_STATUS_NAME        : GMCP_DIS_CHAR_STATUS_NAME,
        GMCP_LBL_CHAR_STATUS_CAPACITY    : GMCP_DIS_CHAR_STATUS_CAPACITY,
        GMCP_LBL_CHAR_STATUS_MAX_CAPACITY: GMCP_DIS_CHAR_STATUS_MAX_CAPACITY,
    ]);

    who->do_gmcp(GMCP_PKG_CHAR_STATUSVARS, data) ;
}

void Status(object who) {
    mapping data ;

    data = ([
        GMCP_LBL_CHAR_STATUS_NAME        : who->query_cap_name(),
        GMCP_LBL_CHAR_STATUS_CAPACITY    : who->query_capacity(),
        GMCP_LBL_CHAR_STATUS_MAX_CAPACITY: who->query_max_capacity(),
    ]) ;

    who->do_gmcp(GMCP_PKG_CHAR_STATUS, data) ;
}
