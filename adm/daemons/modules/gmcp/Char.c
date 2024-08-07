/**
 * @file /adm/daemons/modules/gmcp/Char.c
 * @description Module to handle Char.* GMCP packages
 *
 * @created 2024/02/23 - Gesslar
 * @last_modified 2024/02/23 - Gesslar
 *
 * @history
 * 2024/02/23 - Gesslar - Created
 */

#include <gmcp_defines.h>

inherit STD_DAEMON ;

// This function should present all of the labels that correspond to the
// variables that are sent in the Char.Status package. The purpose of this
// is to inform the client what to display, if desired, for each variable
// on labels in a GUI or messaging resulting from GMCP.
//
// The labels and values are all found in /include/gmcp_defines.h
void StatusVars(object who, mapping payload) {
    mapping data ;

    data = payload || ([
        GMCP_LBL_CHAR_STATUS_NAME        : GMCP_DIS_CHAR_STATUS_NAME,
        GMCP_LBL_CHAR_STATUS_FILL        : GMCP_DIS_CHAR_STATUS_FILL,
        GMCP_LBL_CHAR_STATUS_CAPACITY    : GMCP_DIS_CHAR_STATUS_CAPACITY,
        GMCP_LBL_CHAR_STATUS_XP          : GMCP_DIS_CHAR_STATUS_XP,
        GMCP_LBL_CHAR_STATUS_TNL         : GMCP_DIS_CHAR_STATUS_TNL,
    ]);

    who->do_gmcp(GMCP_PKG_CHAR_STATUSVARS, data) ;
}

void Status(object who, mapping payload) {
    mapping data ;

    data = payload || ([
        GMCP_LBL_CHAR_STATUS_NAME        : who->query_name(),
        GMCP_LBL_CHAR_STATUS_FILL        : sprintf("%d", who->query_fill()),
        GMCP_LBL_CHAR_STATUS_CAPACITY    : sprintf("%d", who->query_capacity()),
        GMCP_LBL_CHAR_STATUS_XP          : sprintf("%d", who->query_xp()),
        GMCP_LBL_CHAR_STATUS_TNL         : sprintf("%d", who->query_tnl()),
    ]) ;

    who->do_gmcp(GMCP_PKG_CHAR_STATUS, data) ;
}

void Vitals(object who, mapping payload) {
    mapping data ;

    data = payload || ([
        GMCP_LBL_CHAR_VITALS_HP     : sprintf("%.2f", who->query_hp()),
        GMCP_LBL_CHAR_VITALS_MAX_HP : sprintf("%.2f", who->query_max_hp()),
        GMCP_LBL_CHAR_VITALS_SP     : sprintf("%.2f", who->query_sp()),
        GMCP_LBL_CHAR_VITALS_MAX_SP : sprintf("%.2f", who->query_max_sp()),
        GMCP_LBL_CHAR_VITALS_MP     : sprintf("%.2f", who->query_mp()),
        GMCP_LBL_CHAR_VITALS_MAX_MP : sprintf("%.2f", who->query_max_mp()),
    ]) ;

    who->do_gmcp(GMCP_PKG_CHAR_VITALS, data) ;
}

void Login(object who, string submodule, mapping payload) {
    switch(submodule) {
        case "Default" : {
            who->do_gmcp(GMCP_PKG_CHAR_LOGIN_DEFAULT, payload) ;
            break ;
        }
    }
}
