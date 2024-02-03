// /adm/daemons/chmodules/error.c
// Channel module for errors
//
// Created:     2024/02/03: Gesslar
// Last Change: 2024/02/03: Gesslar
//
// 2024/02/03: Gesslar - Created

inherit STD_DAEMON ;

private nosave string net = "errorNet";
private nosave string *channels = ({ "error" });
string *history = ({});

void setup() {
     CHAN_D->registerModule(net, file_name());
     filter(channels, (: CHAN_D->registerCh($(net), $1) :));
}

int rec_msg(string chan, string usr, string msg) {
     object ob;

     switch(msg) { /* We could do some neat stuff here! */
          case "/last" : {
               ob = find_player(usr);

               if(!sizeof(history))
                    tell(ob, net + ": Channel " + chan + " has no history yet.\n");
               else
                    tell(ob, implode(history[(sizeof(history) - 15)..(sizeof(history) - 1)], ""));

               return 1;
          }
     }

    if(!adminp(usr)) return 0;

    CHAN_D->rec_msg(chan,  "["  + capitalize(chan) + "] " + capitalize(usr) + ": " + msg + "\n");

    history += ({ ctime(time()) + " [" + capitalize(chan) + "] " + capitalize(usr) + ": " + msg + "\n" });

    return 1;
}

int isAllowed(string channel, string usr, int flag: (: 0 :)) {
     return usr == query_privs(master()) || devp(usr) ;
}
