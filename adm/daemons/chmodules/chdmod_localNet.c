//chdmod_localNet.c

//Tacitus @ LPUniversity
//30-SEPT-05
//Channel Daemon Module - Local Net

//Version 2.10

//Last edited by Parthenon on August 6th, 2006

inherit STD_DAEMON ;

#define SAVE_PATH "/data/daemons/chmodules/localNet.o"

string *ch_list = ({"admin", "wiz", "dev", "gossip", "chat"});
mapping history = ([]);

void setup() {
    set_persistent() ;
}

void post_setup() {
    int i;

    CHAN_D->registerModule("localNet", file_name());

    for(i = 0; i < sizeof(ch_list); i++) {
        CHAN_D->registerCh("localNet", ch_list[i]);
        if(history[ch_list[i]] == 0) history[ch_list[i]] = ({ });
    }

    save_data() ;
}

int rec_msg(string chan, string usr, string msg) {
    object ob;
    string realMsg;
    int numLines;

    if(sscanf(msg, "/last %d", numLines) == 1) {
        if(numLines > sizeof(history[chan]))
            numLines = sizeof(history[chan]);

        ob = find_player(usr);

        if(!sizeof(history[chan]))
            tell_object(ob, "LocalNet: Chael " + chan + " has no history yet.\n");
        else
            foreach(string histLine in history[chan][(sizeof(history[chan]) - numLines)..(sizeof(history[chan]) - 1)])
        tell_object(ob, histLine);
        return 1;
    } else {
        switch(msg) { /* We could do some neat stuff here! */
            case "/last" : {
                    ob = find_player(usr);

                    if(!sizeof(history[chan]))
                        tell_object(ob, "LocalNet: Channel " + chan + " has no history yet.\n");
                    else
                        foreach(string histLine in history[chan][(sizeof(history[chan]) - 15)..(sizeof(history[chan]) - 1)])
                    tell_object(ob, histLine);
                    return 1;
                    break;
                }
            case "/all" : {
                    ob = find_player(usr);

                    if(!sizeof(history[chan]))
                        tell_object(ob, "LocalNet: Channel " + chan + " has no history yet.\n");
                    else
                        foreach(string histLine in history[chan][(sizeof(history[chan]) - 50)..(sizeof(history[chan]) - 1)])
                    tell_object(ob, histLine);
                    return 1;
                    break;
                }
        }
    }

    if(msg[0..0] == ":") {
        msg = msg[1..<1];
        realMsg = sprintf(" %s", msg);
    } else
        realMsg = sprintf(": %s", msg);

    switch(chan) {
        case "admin" : {
                CHAN_D->rec_msg(chan,  "[" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n");
                history[chan] += ({ ctime(time()) + " [" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n" });
                break;
            }

        case "wiz" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n");
                history[chan] += ({ ctime(time()) + " [" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n" });
                break;
            }

        case "gossip" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n");
                history[chan] += ({ ctime(time()) + " [" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n" });
                break;
            }

        case "chat" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n");
                history[chan] += ({ ctime(time()) + " [" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n" });
                break;
            }

        case "dev" : {
                CHAN_D->rec_msg(chan, "[" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n");
                history[chan] += ({ ctime(time()) + " [" + capitalize(chan) + "] " + capitalize(usr) + realMsg + "\n" });
                break;
            }
    }

    save_data() ;
    return 1;
}

int isAllowed(string channel, string usr, int flag: (: 0 :)) {
    if(channel == "admin" && !adminp(usr)) return 0;
    if(channel == "wiz" && !devp(usr)) return 0;
    if(channel == "dev" && !devp(usr)) return 0;
    return 1;
}
