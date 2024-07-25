/**
 * @file /adm/daemons/modules/channel/error.c
 * @description Channel module for errors
 *
 * @created 2024/02/03 - Gesslar
 * @last_modified 2024/02/03 - Gesslar
 *
 * @history
 * 2024/02/03 - Gesslar - Created
 */

inherit STD_DAEMON ;

private nosave string module_name = query_file_name(this_object()) ;
private nosave string *channels = ({ "error" });
string *history = ({});

void setup() {
     mixed result ;

     set_no_clean(1) ;

     CHAN_D->register_module(module_name, file_name());
     result = filter(channels, (: CHAN_D->register_channel($(module_name), $1) :));
}

int rec_msg(string chan, string usr, string msg) {
     object ob;

     switch(msg) { /* We could do some neat stuff here! */
          case "/last" : {
               ob = find_player(usr);

               if(!sizeof(history))
                    tell(ob, module_name + ": Channel " + chan + " has no history yet.\n");
               else
                    tell(ob, implode(history[(sizeof(history) - 15)..(sizeof(history) - 1)], ""));

               return 1;
          }
     }

    if(!adminp(usr)) return 0;

    CHAN_D->rec_msg(chan,  "["  + capitalize(chan) + "] " + capitalize(usr) + ": " + msg + "\n");

    history += ({ ldate(time(),1) +" "+ltime() + " [" + capitalize(chan) + "] " + capitalize(usr) + ": " + msg + "\n" });

    return 1;
}

int is_allowed(string channel, string usr, int flag: (: 0 :)) {
     return usr == query_privs(master()) || devp(usr) ;
}
