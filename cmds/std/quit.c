//quit.c

//Tacitus @ LPUniversity
//06-APR-05
//User command

//Last edited on October 8th, 2005 by Tacitus

#include <logs.h>

inherit STD_CMD ;

mixed main(object caller, string arg) {
     object user = caller->query_user() ;
     object poss ;

     if(user) {
          if(user->query_su_body()) {
               poss = user->revert() ;
               if(poss) {
                    tell(poss, "You return to your body.\n") ;
                    poss->other_action("$O $vreturn to $p body.", caller) ;
               } else {
                    tell(caller, "Failed to return to your body.\n") ;
                    return 1 ;
               }

               // This is a hack because otherwise the privileges won't work
               // for saving player data, because this_interactive() will be
               // the object that called quit, instead of the player object.
               call_out_walltime((:$(poss)->force_me("quit") :), 0.01) ;
               return 1 ;
          }
     }

     tell(caller, "Thank you for visiting " + mud_name() + ".\n");
     caller->exit_world();
     log_file(LOG_LOGIN, capitalize(caller->query_real_name()) + " logged out from " +
     query_ip_number(caller) + " on " + ctime(time()) + "\n");
     destruct(caller);
     return 1;
}

string help(object caller) {
     return(" SYNTAX: quit\n\n" +
     "This command will save your characher and disconnect you from the mud.\n\n" +
     "See also: save\n");
}
