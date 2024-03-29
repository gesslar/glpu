#include "/adm/obj/simul_efun.h"

//security.c

//Tacitus @ LPUniversity
//Grouped on October 22nd, 2005

int is_member(string user, string group) {
     if(master()->is_member(user, group)) return 1;
     else return 0;
}

int adminp(mixed user) {
     if(!user) user = previous_object() ;
     if(stringp(user)) {
          if(is_member(user, "admin")) return 1;
     }
     else if(is_member(query_privs(user), "admin")) return 1;
     else return 0;
}

varargs int devp(mixed user) {
     if(!user) user = previous_object();
     if(stringp(user)) {
          if(is_member(user, "developer")) return 1;
     }
     else if(is_member(query_privs(user), "developer")) return 1;
     else return 0;
}

int wizardp(mixed user) { return devp(user); }
