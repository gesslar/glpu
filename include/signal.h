#ifndef __SIGNAL_H__
#define __SIGNAL_H__

// These values are for slot regsitration
#define SIG_SLOT_OK                   1
#define SIG_SLOT_INVALID_CALLER       0
#define SIG_MISSING_SIGNAL           -1
#define SIG_MISSING_OBJECT           -2
#define SIG_MISSING_FUNCTION         -3
#define SIG_INVALID_FUNCTION         -4
#define SIG_INVALID_OBJECT           -5

// Standard system signals
#define SIG_SYS_BOOT                100
#define SIG_SYS_CRASH               101
#define SIG_SYS_SHUTTING_DOWN       102
#define SIG_SYS_SHUTDOWN            103
#define SIG_SYS_SHUTDOWN_CANCEL     104
#define SIG_SYS_REBOOTING           105
#define SIG_SYS_REBOOT_CANCEL       106
#define SIG_SYS_PERSIST             107

// Standard user signals
#define SIG_USER_LOGIN              200
#define SIG_USER_LOGOUT             201
#define SIG_USER_LINKDEAD           202
#define SIG_USER_LINK_RESTORE       203

// Player signals
#define SIG_PLAYER_DIED             300
#define SIG_PLAYER_REVIVED          301
#define SIG_PLAYER_ADVANCED         302
#define SIG_USER_ENV_VAR_CHANGED    303

// Game Signals
#define SIG_GAME_MIDNIGHT           400

#endif // __SIGNAL_H__
