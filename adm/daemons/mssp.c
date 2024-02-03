// /adm/daemons/mssp.c
// This daemon is called by the master object to respond to the get_mud_stats()
// apply.
//
// Created:     2024/02/03: Gesslar
// Last Change: 2024/02/03: Gesslar
//
// 2024/02/03: Gesslar - Created

#include <runtime_config.h>

inherit STD_DAEMON ;

private nosave mapping mud_stats = ([]) ;

void setup() {
    mud_stats = ([
        "NAME"                  : mud_name(),
        "PORT"                  : sprintf("%d", __PORT__),
        "UPTIME"                : sprintf("%d", time() - uptime()),
        "PLAYERS"               : sprintf("%d", sizeof( users() )),
        "GMCP"                  : sprintf("%d", get_config(__RC_ENABLE_GMCP__)),
        "MCP"                   : "0",
        "GMCP"                  : sprintf("%d", get_config(__RC_ENABLE_GMCP__)),
        "MXP"                   : sprintf("%d", get_config(__RC_ENABLE_MXP__)),
        "MSP"                   : sprintf("%d", get_config(__RC_ENABLE_MSP__)),
        "MCCP"                  : "1",
        "UTF-8"                 : "1",
    ]) +
    json_decode(read_file("/adm/etc/mssp.json")) ;
}

mapping get_mud_stats() {
    return copy(mud_stats) ;
}
