#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <clean.h>
#include <daemons.h>
#include <input.h>
#include <inventory.h>
#include <messaging.h>
#include <modules.h>
#include <move.h>
#include <mudlib.h>
#include <objects.h>
#include <runtime_config.h>
#include <signal.h>

#define SIMUL_OB     "/adm/obj/simul_efun"
#define LOGIN_OB     "/adm/obj/login"
#define ROOM_VOID    "/d/std/void"
#define ROOM_FREEZER "/d/std/freezer"
#define ROOM_START   "/d/village/square"

#define true 1
#define false 0
#define null ([])[0]
#define undefined ([])[0]

#define DATE "%F"
#define TIME "%T"
#define DATE_TIME "%F %T"

#endif // __GLOBAL_H__
