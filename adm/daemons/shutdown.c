//shutdown_d.c

//Tactius @ LPUniversity
//22-OCT-05
//Shutdown daemon

//Nothing fancy, but it works :) -- Tacitus

//Last edited on October 23rd, 2005 by Tacitus

#include <logs.h>
#include <commands.h>
#include <shutdown.h>

inherit STD_DAEMON ;

int start(int minutes, int type);
private nomask void do_shutdown() ;
int stop();
void check();
string get_status();

int start_time;
int end_time;
int type;
int running = 0;

int start(int minutes, int type_in) {
    int i;
    object *users = users();

    type = type_in;
    if(!caller_is(CMD_SHUTDOWN) && !caller_is(CMD_REBOOT))
        return 0;

    if(find_call_out(running) > -1)
        return 0 ;

    message("warning",
        sprintf("%s will be %s %s.\n",
            mud_name(),
            type == SYS_SHUTDOWN ? "shutting down" : "rebooting",
            minutes ? "in " + minutes +" minutes" : "now"),
        users()
    );

    if(minutes == 0) {
        do_shutdown();
    } else {
        start_time = time();
        end_time = time() + (60 * minutes);
        running = call_out("do_shutdown", minutes * 60);

        if(type == SYS_SHUTDOWN)
            emit(SIG_SYS_SHUTTING_DOWN, end_time-start_time) ;
        else
            emit(SIG_SYS_REBOOTING, end_time-start_time) ;

        call_out("check", 1);
    }
    return 1;
}

int stop() {
    if(find_call_out(running) == -1)
        return 0 ;

    if(!caller_is(CMD_SHUTDOWN) && !caller_is(CMD_REBOOT))
        return 0;

    message("warning", "Shutdown/Reboot Canceled.\n", users());
    remove_call_out() ;
    running = 0 ;

    if(type == SYS_SHUTDOWN)
        emit(SIG_SYS_SHUTDOWN_CANCEL) ;
    else
        emit(SIG_SYS_REBOOT_CANCEL) ;

    return 1;
}

private nomask void do_shutdown() {
    int i;
    object *users = users();

    message("warning", "Warning: "+mud_name()+" is going down now!\n", users());

    log_file(LOG_SHUTDOWN, sprintf(ctime(time()) + ": " + mud_name() + "%s", type == SYS_SHUTDOWN ? " shutting down.\n" : " rebooting.\n"));

    for(i = 0; i < sizeof(users); i ++)
        users[i]->save_user();

    log_file(LOG_LOGIN, sprintf("Driver %s on " + ctime(time()) + ". All users forced to quit.\n", type == SYS_SHUTDOWN ? "shut down" : "rebooted"));

    shutdown(type) ;
}

void check() {
    int time_left;
    int i;
    object *users = users();
    int remaining ;

    if((time_left = find_call_out(running)) == -1)
        return;

    if((time_left % 300) == 0 && time_left < 1200 || time_left == 30 || time_left == 10) {
        message("warning",
            sprintf("%s will be %s %s %s.\n",
                mud_name(),
                type == SYS_SHUTDOWN ? "shutting down" : "rebooting",
                "in " + (time_left / 60)  + " minutes and",
                (time_left % 60) + " seconds"),
            users()
        );
    }

    if(type == SYS_SHUTDOWN)
        emit(SIG_SYS_SHUTTING_DOWN, time_left) ;
    else
        emit(SIG_SYS_REBOOTING, time_left) ;

    call_out("check", 1);
}

string get_status() {
    if(find_call_out(running) == -1)
        return null ;

    return sprintf("%s will be %s in %d minutes and %d seconds.\n", mud_name(), type == SYS_SHUTDOWN ? "shutting down" : "rebooting", (end_time - time()) / 60, (end_time - time()) % 60);
}
