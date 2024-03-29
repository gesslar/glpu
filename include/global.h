//global.h

//Tacitus @ LPUniversity
//02-APR-05
//LPUniversity

//Last updated: October 9th, 2006 by Tacitus
// Last Change: 2024/02/03: Gesslar

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <config.h>
#include <daemons.h>
#include <modules.h>
#include <messaging.h>
#include <mudlib.h>
#include <objects.h>
#include <runtime_config.h>
#include <move.h>
#include <targeting.h>

#define SIMUL_DIR  "/adm/simul_efun/"
#define SIMUL_OB   "/adm/obj/simul_efun"
#define LOGIN_OB   "/adm/obj/login"
#define VOID_OB    "/areas/std/void"

#define true 1
#define false 0
#define null ([])[0]
#define undefined ([])[0]

#define DATE "%F"
#define TIME "%T"
#define DATE_TIME "%F %T"

#endif // __GLOBAL_H__
