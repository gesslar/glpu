//chdmod_local_net.c

//Tacitus @ LPUniversity
//30-SEPT-05
//Channel Daemon Module - Local Net

//Version 2.10

//Last edited by Parthenon on August 6th, 2006

inherit STD_DAEMON ;

string *ch_list = ({"admin", "wiz", "dev", "gossip", "chat"});
mapping history = ([]);
private nosave string module_name = query_file_name(this_object()) ;

void setup() {
    set_persistent() ;
}

void post_setup() {
    int i;

    CHAN_D->register_module(module_name, file_name());

    for(i = 0; i < sizeof(ch_list); i++) {
        CHAN_D->register_channel(module_name, ch_list[i]);
        if(history[ch_list[i]] == 0) history[ch_list[i]] = ({ });
    }

    save_data() ;
}

int rec_msg(string chan, string usr, string msg) {
    object ob;
    string real_message;
    int num_lines;

    if(sscanf(msg, "/last %d", num_lines) == 1) {
        if(num_lines > sizeof(history[chan]))
            num_lines = sizeof(history[chan]);

        ob = find_player(usr);

        if(!sizeof(history[chan]))
            tell_object(ob, "LocalNet: Chael " + chan + " has no history yet.\n");
        else
            foreach(string hist_line in history[chan][(sizeof(history[chan]) - num_lines)..(sizeof(history[chan]) - 1)])
        tell_object(ob, hist_line);
        return 1;
    } else {
        switch(msg) { /* We could do some neat stuff here! */
            case "/last" : {
                    ob = find_player(usr);

                    if(!sizeof(history[chan]))
                        tell_object(ob, "LocalNet: Channel " + chan + " has no history yet.\n");
                    else
                        foreach(string hist_line in history[chan][(sizeof(history[chan]) - 15)..(sizeof(history[chan]) - 1)])
                    tell_object(ob, hist_line);
                    return 1;
                    break;
                }
            case "/all" : {
                    ob = find_player(usr);

                    if(!sizeof(history[chan]))
                        tell_object(ob, "LocalNet: Channel " + chan + " has no history yet.\n");
                    else
                        foreach(string hist_line in history[chan][(sizeof(history[chan]) - 50)..(sizeof(history[chan]) - 1)])
                    tell_object(ob, hist_line);
                    return 1;
                    break;
                }
        }
    }

    if(msg[0..0] == ":") {
        msg = msg[1..<1];
        real_message = sprintf(" %s", msg);
    } else
        real_message = sprintf(": %s", msg);

    switch(chan) {
        case "admin" : {
                CHAN_D->rec_msg(chan,  "[" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n");
                history[chan] += ({ ldate(time(),1) +" "+ltime() + " [" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n" });
                break;
            }

        case "wiz" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n");
                history[chan] += ({ ldate(time(),1) +" "+ltime() + " [" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n" });
                break;
            }

        case "gossip" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n");
                history[chan] += ({ ldate(time(),1) +" "+ltime() + " [" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n" });
                break;
            }

        case "chat" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n");
                history[chan] += ({ ldate(time(),1) +" "+ltime() + " [" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n" });
                break;
            }

        case "dev" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n");
                history[chan] += ({ ldate(time(),1) +" "+ltime() + " [" + capitalize(chan) + "] " + capitalize(usr) + real_message + "\n" });
                break;
            }
    }

    save_data() ;
    return 1;
}

int is_allowed(string channel, string usr, int flag: (: 0 :)) {
    if(channel == "admin" && !adminp(usr)) return 0;
    if(channel == "wiz" && !devp(usr)) return 0;
    if(channel == "dev" && !devp(usr)) return 0;
    return 1;
}
