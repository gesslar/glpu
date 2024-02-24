#ifndef __GMCP_H__
#define __GMCP_H__

// Packages, subpackges, and commands
#define GMCP_PKG_CORE                       "Core"
#define GMCP_PKG_CORE_HELLO                 GMCP_PKG_CORE".Hello"
#define GMCP_PKG_CORE_PING                  GMCP_PKG_CORE".Ping"
#define GMCP_PKG_CORE_SUPPORTS              GMCP_PKG_CORE".Supports"

#define GMCP_PKG_CHAR                       "Char"
#define GMCP_PKG_CHAR_STATUS                GMCP_PKG_CHAR".Status"
#define GMCP_PKG_CHAR_STATUSVARS            GMCP_PKG_CHAR".StatusVars"
#define GMCP_PKG_CHAR_VITALS                GMCP_PKG_CHAR".Vitals"

#define GMCP_PKG_COMM                       "Comm"
#define GMCP_PKG_COMM_CHANNEL               GMCP_PKG_COMM".Channel"
#define GMCP_PKG_COMM_CHANNEL_LIST          GMCP_PKG_COMM_CHANNEL".List"
#define GMCP_PKG_COMM_CHANNEL_TEXT          GMCP_PKG_COMM_CHANNEL".Text"

#define GMCP_PKG_EXTERNAL                    "External"
#define GMCP_PKG_EXTERNAL_DISCORD            GMCP_PKG_EXTERNAL".Discord"
#define GMCP_PKG_EXTERNAL_DISCORD_HELLO      GMCP_PKG_EXTERNAL_DISCORD".Hello"
#define GMCP_PKG_EXTERNAL_DISCORD_GET        GMCP_PKG_EXTERNAL_DISCORD".Get"

// Labels and display values
#define GMCP_LBL_CHAR_STATUS_NAME           "name"
#define GMCP_DIS_CHAR_STATUS_NAME           "Name"
#define GMCP_LBL_CHAR_STATUS_CAPACITY       "capacity"
#define GMCP_DIS_CHAR_STATUS_CAPACITY       "Capacity"
#define GMCP_LBL_CHAR_STATUS_MAX_CAPACITY   "max_capacity"
#define GMCP_DIS_CHAR_STATUS_MAX_CAPACITY   "Max Capacity"

#endif // __GMCP_H__
