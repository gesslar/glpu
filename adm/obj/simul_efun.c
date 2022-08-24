//simul_efun.c

//Tacitus @ LPUniversity
//02-APR-05
//Simul-efun object

// Last Change: 2022/08/23: Gesslar
// Order does not matter.
// 1. When adding new simul_efuns, prototype them in /adm/obj/simul_efun.h
// 2. When adding new files to the simuL_efun include list, in that file,
//    #include "/adm/obj/simul_efun.h"

#pragma show_error_context

#include "/adm/simul_efun/security.c"
#include "/adm/simul_efun/overrides.c"
#include "/adm/simul_efun/system.c"
#include "/adm/simul_efun/exists.c"
#include "/adm/simul_efun/user.c"
#include "/adm/simul_efun/resolve_path.c"
#include "/adm/simul_efun/file.c"
#include "/adm/simul_efun/object.c"
#include "/adm/simul_efun/socket.c"
#include "/adm/simul_efun/directory.c"
