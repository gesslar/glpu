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

#define LIB_VERSION "A-0610-R7"

#define SIMUL_DIR  "/adm/simul_efun/"
#define SIMUL_OB   "/adm/obj/simul_efun"
#define LOGIN_OB   "/adm/obj/login"
#define USER_OB    "/std/user/user"
#define VOID_OB    "/areas/std/void"

#define TRUE 1
#define FALSE 0
#define UNDEFINED ([])[0]

#endif // __GLOBAL_H__
