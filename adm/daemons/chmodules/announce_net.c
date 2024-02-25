/* chdmod_announce.c

 Tacitus @ LPUniversity
 28-JUNE-06
 Announce channel module

*/

inherit STD_DAEMON ;

string *history = ({});
string net_name = query_file_name(this_object()) ;

void setup() {
     CHAN_D->register_module("announce_net", file_name(this_object()));
     CHAN_D->register_channel(net_name, "announce");
}

int rec_msg(string chan, string usr, string msg) {
     object ob;

     switch(msg) { /* We could do some neat stuff here! */
          case "/last" : {
               ob = find_player(usr);

               if(!sizeof(history)) tell_object(ob, net_name + ": "
                "Channel " + chan + " has no history yet.\n");
               else tell_object(ob,
                    implode(history[(sizeof(history) - 15)..(sizeof(history) - 1)], ""));

               return 1;
          }
     }

    if(!adminp(usr)) return 0;

    CHAN_D->rec_msg(chan,  "["  + capitalize(chan) + "] " + capitalize(usr) + ": " + msg + "\n");

    history += ({ ldate(time(),1) +" "+ltime() + " [" + capitalize(chan) + "] " +
            capitalize(usr) + ": " + msg + "\n" });

    return 1;
}

void announce_login(string user) {
    CHAN_D->rec_msg("announce",
         "[Announce] System: "
             + capitalize(user) + " has logged into "
            + mud_name() + ".\n");
    history += ({ ctime(time())
        + " [Announce] System: "
             + capitalize(user) + " has logged into "
            + mud_name() + ".\n" });
}

void announce_logoff(string user) {
    CHAN_D->rec_msg("announce",
         "[Announce] System: "
             + capitalize(user) + " has left "
            + mud_name() + ".\n");
    history += ({ ctime(time())
        + " [Announce] System: "
             + capitalize(user) + " has left "
            + mud_name() + ".\n" });
}

int is_allowed(string channel, string usr, int flag: (: 0 :)) {
    return 1;
}
