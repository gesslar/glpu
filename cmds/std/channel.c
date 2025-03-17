/* channel.c

 Tacitus @ LPUniversity
 02-OCT-05
 Standard Command

 - Rewrote 15-JAN-06 by Tacitus @ LPUni
*/

/* Last edited on July 14th, 2006 by Tacitus */

inherit STD_CMD;

// Function declarations
private string format_channel_list(string *channels);
private void display_channels_by_module(object tp);
private void display_channel_members(object tp, string channel);
private void display_tuned_channels(object tp, string _name);
public varargs int tune(string channel, string _name, int in_out, int silent);

mixed main(object tp, string args) {
    string cmd, arg;
    string _name;

    _name = query_privs(tp);

    if(!args) {
        display_tuned_channels(tp, _name);
        return 1;
    }

    if(sscanf(args, "%s %s", cmd, arg) != 2)
        cmd = args;

    switch(cmd) {
        case "list":
            display_channels_by_module(tp);
            return 1;

        case "show":
            if(!arg)
                return "Syntax: channel show <channel_name>\n";
            display_channel_members(tp, arg);
            return 1;

        case "tune": {
            string direction, channel;
            if(!arg)
                return "Syntax: channel tune <in/out> <channel/network/all>\n";
            if(sscanf(arg, "%s %s", direction, channel) != 2)
                return "Syntax: channel tune <in/out> <channel/network/all>\n";
            tune(channel, _name, direction == "in", 0);
            return 1;
        }
        case "tuned":
            display_tuned_channels(tp, arg || _name);
            return 1;

        default:
            return "Syntax: channel [list/show/tune/tuned] [argument]\n";
    }
}

// Formats a list of channels for display
private string format_channel_list(string *channels) {
    if(sizeof(channels) == 1)
        return channels[0];
    else if(sizeof(channels) > 7)
        return sprintf("%s,\n\t%s, and %s", implode(channels[0..6], ", "),
            implode(channels[7..<2], ", "), channels[<1]);
    else
        return sprintf("%s, and %s", implode(channels[0..<2], ", "), channels[<1]);
}

// Displays all channels grouped by their modules
private void display_channels_by_module(object tp) {
    string *modules;
    string output;
    int i;
    string _name = query_privs(tp);

    modules = sort_array(CHAN_D->get_modules(), 1);
    output = "Channels by module:\n";

    for(i = 0; i < sizeof(modules); i++) {
        string *channels = sort_array(CHAN_D->get_channels(modules[i], _name), 1);
        if(sizeof(channels) > 0) {
            output += sprintf("%s - %s\n", modules[i], implode(channels, ", "));
        }
    }

    tell(tp, output);
}

// Displays all members tuned into a specific channel
private void display_channel_members(object tp, string channel) {
    string *members;

    if(!CHAN_D->valid_channel(channel))
        return tell(tp, "Channel: Channel " + channel + " does not exist.\n");

    members = CHAN_D->get_tuned(channel);
    if(sizeof(members) > 0) {
        tell(tp, sprintf("Users tuned into '%s':\n\t%s\n",
            channel, format_channel_list(map(members, (: capitalize :)))));
    } else {
        tell(tp, "Channel: No users tuned into channel " + channel + "\n");
    }
}

// Displays all channels a user is tuned into
private void display_tuned_channels(object tp, string _name) {
    string *all_channels, *tuned_channels;
    int i;

    if(_name != query_privs(tp) && !wizardp(tp)) {
        _name = query_privs(tp);
    }

    all_channels = CHAN_D->get_channels("all", _name);
    tuned_channels = ({});

    for(i = 0; i < sizeof(all_channels); i++) {
        string *members = CHAN_D->get_tuned(all_channels[i]);
        if(member_array(_name, members) != -1) {
            tuned_channels += ({ all_channels[i] });
        }
    }

    if(sizeof(tuned_channels) > 0) {
        tell(tp, sprintf("%s currently tuned into:\n\t%s\n",
            (_name == query_privs(tp) ? "You are" : capitalize(_name) + " is"),
            format_channel_list(tuned_channels)));
    } else {
        tell(tp, sprintf("%s not currently tuned into any channels.\n",
            (_name == query_privs(tp) ? "You are" : capitalize(_name) + " is")));
    }
}

// Tunes a user in or out of a channel
public varargs int tune(string channel, string _name, int in_out, int silent) {
    string *channels;
    int result, i;

    if(nullp(channel) || !stringp(channel) || nullp(_name) || !stringp(_name))
        return 0;

    result = 1;

    if(channel == "all") {
        channels = CHAN_D->get_channels("all", _name);
        if(sizeof(channels) == 0) {
            if(!silent)
                tell(this_player(), "No channels available to tune.\n");
            return 0;
        }
    } else {
        channels = ({ channel });
    }

    for(i = 0; i < sizeof(channels); i++) {
        int tune_result;

        tune_result = CHAN_D->tune(channels[i], _name, in_out);
        if(!silent) {
            if(tune_result)
                tell(this_player(), sprintf("%s channel tuned %s.\n",
                    capitalize(channels[i]),
                    in_out ? "in" : "out"));
            else
                tell(this_player(), sprintf("Channel %s does not exist.\n", channels[i]));
        }
        result = result && tune_result;
    }

    return result;
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
