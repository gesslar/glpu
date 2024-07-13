// /cmds/std/gmcp.c
// Basic command for ineracting with the GMCP system
//
// Created:     2024/02/23: Gesslar
// Last Change: 2024/02/23: Gesslar
//
// 2024/02/23: Gesslar - Created

inherit STD_CMD ;

mixed gmcp_status(object tp) ;

mixed main(object tp, string arg) {
    if(!get_config(__RC_ENABLE_GMCP__))
        return "GMCP is not enabled on the server." ;

    if(!arg)
        return gmcp_status(tp) ;

    if(arg == "on") {
        if(tp->gmcp_enabled()) {
            return "GMCP is already enabled." ;
        }
        if(!has_gmcp(tp)) {
            return "GMCP is not available. Ensure you are using a GMCP "
                "capable client and that GMCP is enabled in the client." ;
        }
        tp->set_env("gmcp", "on") ;
        GMCP_D->init_gmcp(tp) ;
        return "GMCP is now enabled." ;
    } else if(arg == "off") {
        if(!tp->gmcp_enabled()) {
            return "GMCP is already disabled." ;
        }
        if(!has_gmcp(tp)) {
            return "GMCP is not available. Ensure you are using a GMCP "
                "capable client and that GMCP is enabled in the client." ;
        }
        tp->set_env("gmcp", "off") ;
        return "GMCP is now disabled." ;
    }

    return "Syntax: gmcp OR gmcp [on|off]" ;
}

mixed gmcp_status(object tp) {
    if(tp->gmcp_enabled()) {
        return "GMCP is currently enabled." ;
    }
    if(!has_gmcp(tp)) {
        return "GMCP is not available. Ensure you are using a GMCP "
            "capable client and that GMCP is enabled in the client." ;
    }
    return "GMCP is currently disabled." ;
}

string help(object who) {
    return
"Syntax: gmcp\n"
"        gmcp <on/off>\n\n"
"Displays your current GMCP status or enables/disables GMCP. GMCP is a "
"protocol that allows the server to send information to the client. This "
"information can be used to enhance the user interface and provide additional "
"information to the player. GMCP is only available if the client supports it "
"and it is enabled in the client and the server." ;
}
