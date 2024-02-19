//shutdown_d.c

//Tactius @ LPUniversity
//22-OCT-05
//Shutdown daemon

//Nothing fancy, but it works :) -- Tacitus

//Last edited on October 23rd, 2005 by Tacitus

#include <logs.h>

inherit STD_DAEMON ;

int start(int minutes, int type);
int stop();
void check();
string get_status();

int start_time;
int end_time;
int type;
int true = 0;

int start(int minutes, int type_in) {
     int i;
     object *users = users();

     type = type_in;
     if(previous_object() != find_object("/cmds/adm/shutdown.c") && previous_object() != find_object("/cmds/adm/reboot.c")) return 0;
     if(true) return 0;

     message("warning",
          sprintf("The mud will be %s %s.\n",
               type ? "shutting down" : "rebooting",
               minutes ? "in " + minutes +" minutes" : "now"),
          users()
     );

     if(minutes == 0)
     {
          log_file(LOG_SHUTDOWN, sprintf(ctime(time()) + ": " + mud_name() + "%s", type ? " shutting down.\n" : " rebooting.\n"));
          log_file(LOG_LOGIN, sprintf("Driver %s on " + ctime(time()) + ". All users forced to quit.\n", type ? "shut down" : "rebooted"));
          for(i = 0; i < sizeof(users); i ++) users[i]->save_user();
                message("warning", "Warning: The mud is going down now!\n", users());
          type ? shutdown(-1) : shutdown(0);
     }

     else
     {
          start_time = time();
          end_time = time() + (60 * minutes);
          true = 1;
          call_out("check", 1);
     }
     return 1;
}

int stop()
{
     if(!true) return 0;
     if(previous_object() != find_object("/cmds/adm/shutdown.c") && previous_object() != find_object("/cmds/adm/reboot.c")) return 0;
     message("warning", "Shutdown/Reboot Canceled.\n", users());
     remove_call_out("check");
     true = 0;
     return 1;

}

void check()
{
     int time_left;
     int i;
     object *users = users();
     if(!true) return;
     time_left = end_time - time();
     if(time_left <= 0)
     {
          message("warning", "Warning: The mud is going down now!\n", users());
          log_file(LOG_SHUTDOWN, sprintf(ctime(time()) + ": " + mud_name() + "%s", type ? " shutting down.\n" : " rebooting.\n"));
          for(i = 0; i < sizeof(users); i ++) users[i]->save_user();
          log_file(LOG_LOGIN, sprintf("Driver %s on " + ctime(time()) + ". All users forced to quit.\n", type ? "shut down" : "rebooted"));
          type ? shutdown(-1) : shutdown(0);
     }

     if((time_left % 300) == 0 && time_left < 1200 || time_left == 30 || time_left == 10)
     {
          message("warning",
          sprintf("The mud will be %s %s %s.\n",
               type ? "shutting down" : "rebooting",
               "in " + (time_left / 60)  + " minutes and",
               (time_left % 60) + " seconds"),
          users()
          );

     }
     call_out("check", 1);

}

string get_status()
{
     if(true) return sprintf("The mud will be %s in %d minutes and %d seconds.\n", type ? "shutting down" : "rebooting", (end_time - time()) / 60, (end_time - time()) % 60  );
}
