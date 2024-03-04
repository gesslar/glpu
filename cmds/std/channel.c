/* channel.c

 Tacitus @ LPUniversity
 02-OCT-05
 Standard Command

 - Rewrote 15-JAN-06 by Tacitus @ LPUni
*/

/* Last edited on July 14th, 2006 by Tacitus */

inherit STD_CMD ;

mixed main(object tp, object room, string args) {
    string cmd, arg, arg_channel;
    string *arr, multiple_tune;
    int i;
    string name = query_privs(tp);

    if(!args)
        return "Sytnax: channel <list/show/tune> <argument>\n";

    sscanf(args, "%s", cmd);
    sscanf(args, "%s %s", cmd, arg);
    sscanf(args, "%s %s %s", cmd, arg, arg_channel);

    switch(cmd) {
        case "list" :
        {
            if(!arg) {
                arr = CHAN_D->get_modules();

                if(sizeof(arr) == 1)
                    tell(tp, "The following channel module is installed:\n\t" + arr[0] + "\n");
                else
                    tell(tp, "The following channel modules are installed: \n\t"
                        + implode(arr[0..(sizeof(arr) - 2)], ", ") + ", " + arr[sizeof(arr) - 1]
                        + "\n");

                tell(tp, "\nYou may list the channels registered by each module by typing:\n");
                    tell(tp, "\tchannel list [<module_name> || all]\n");
                    return 1;
            }

            arr = CHAN_D->get_channels(arg);
            if(sizeof(arr) > 0) {
                if(sizeof(arr) == 1)
                    tell(tp, "The channels are as follows for '"+ arg + "':\n\t" + arr[0] + "\n");
                else if(sizeof(arr) > 7)
                    tell(tp, sprintf("The channels are as follows for '" + arg + "':\n\t%s,\n\t%s, and %s\n", implode(arr[0..6], ", "),
                         implode(arr[7..(sizeof(arr)-2)], ", "), arr[sizeof(arr)-1]));
                else
                    tell(tp, sprintf("The channels are as follows for '" + arg
                        + "':\n\t%s, and %s.\n", implode(arr[0..sizeof(arr)-2], ", "),
                         arr[sizeof(arr)-1]));
                return 1;
            }
            tell(tp, "Channel: No channels found for group " + arg + "\n");
            return 1;
        }
        case "show" :
        {
            if(!arg)
                return "Sytnax: channel <list/show/tune> <argument>\n";
            if(!CHAN_D->valid_ch(arg))
                return notify_fail("Channel: Channel " + arg + " does not exist.\n");
            arr = CHAN_D->get_tuned(arg);
            if(sizeof(arr) > 0) {
                if(sizeof(arr) == 1)
                    tell(tp, "The the following user is tuned into '" + arg + "':\n\t" + capitalize(arr[0]) + "\n");
                else
                    tell(tp, sprintf("The following users are tuned into '" + arg + "':\n\t%s, and %s.\n", implode(arr[0..sizeof(arr)-2], ", "), arr[sizeof(arr)-1]));
                    return 1;
            }
            tell(tp, "Channel: No users tuned into channel " + arg + "\n");
            return 1;
        }
        case "tune" :
        {
            if(!arg_channel)
                return "Syntax: channel tune <in/out> <channel/network>\n";

            if(arg == "in") {
                multiple_tune = CHAN_D->get_channels(arg_channel);
                if(sizeof(multiple_tune) > 0) {
                    for(i = 0; i < sizeof(multiple_tune); i ++) {
                        if(CHAN_D->tune(multiple_tune[i], name, 1))
                            tell(tp, "Tune: Tuned into channel " + multiple_tune[i] + "\n");
                        else
                            tell(tp, "Tune: Channel " + multiple_tune[i] + " does not exist.\n");
                    }
                    return 1;
                }
                if(CHAN_D->tune(arg_channel, name, 1))
                    tell(tp, "Tune: Tuned into channel " + arg_channel + "\n");
                else
                    tell(tp, "Tune: Channel " + arg_channel + " does not exist.\n");
                return 1;
            }

            if(arg == "out") {
                multiple_tune = CHAN_D->get_channels(arg_channel);
                if(sizeof(multiple_tune) > 0) {
                    for(i = 0; i < sizeof(multiple_tune); i ++) {
                        if(CHAN_D->tune(multiple_tune[i], name, 0))
                            tell(tp, "Tune: Tuned out of channel " +  multiple_tune[i] + "\n");
                        else
                            tell(tp, "Tune: Channel " + multiple_tune[i] + " does not exist.\n");
                    }
                    return 1;
                }
                if(CHAN_D->tune(arg_channel, name, 0))
                    tell(tp, "Tune: Tuned out of channel " + arg_channel + "\n");
                else
                    tell(tp, "Tune: Channel " + arg_channel + " does not exist.\n");
                return 1;
            }
        }

        default :
        {
            return "Sytnax: channel <list/show/tune> <argument>\n";
        }
    }
}

string help(object tp) {
    string *mod_list = CHAN_D->get_modules(), mods;

    if(!sizeof(mod_list)) mods = "There is no networks currently installed.";
    if(sizeof(mod_list) == 1) mods = mod_list[0];
    else mods = implode(mod_list[0..(sizeof(mod_list)-2)], ", ") + ", " + mod_list[sizeof(mod_list) - 1];
    return(" SYNTAX: channel <list/show/tune> <argument>\n\n"
    "This command allows you to interact with the different channel networks\n"
    "available here on " + capitalize(mud_name()) + ". More specifically,\n"
    "it currently allows you to list the different channels that you can\n"
    "tune into, display who is tuned into a channel, and the ability to\n"
    "tune in and out of channels/networks. If you are wondering what a\n"
    "network is, it is a group of channels. You may notice that different\n"
    "networks have different channels, different features, and different\n"
    "users/muds communicating through it. Currently, the following networks\n"
    "are installed on " + capitalize(mud_name()) + ":\n" +
    "\t" + mods + "\n");
}
