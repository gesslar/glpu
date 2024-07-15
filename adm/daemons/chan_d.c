/* chan_d.c

 Tacitus @ LPUniversity
 30-SEPT-05
 Modular Channel Daemon

 Version 2.5

*/

inherit STD_DAEMON ;

/* Last modified by Tacitus on July 5th, 2006 */

int register_module(string name, string path);
int register_channel(string module_name, string channel_name);
int remove_module(string module_name);
int remove_channel(string channel_name);
int tune(string channel, string user, int flag);
int valid_ch(string channel_name);
int valid_module(string module_name);
int chat(string channel, string user, string msg);
void muddy_chat(mapping payload);
int filter_listing(string element);
string *get_channels(string module_name);
string *get_modules();
string *get_tuned(string argument);
void rec_msg(string channel, string msg);

mapping channels;
mapping modules;

void setup() {
    string str, err, *arr;
    int i;
    float time;
    object ob ;

    set_no_clean() ;

    channels = ([]);
    modules = ([]);

    arr = explode_file("/adm/daemons/modules/channel/installed_modules");

    if(!sizeof(arr))
        return ;

    for (i = 0; i < sizeof(arr); i++) {
        string out = "" ;
        if(ob = find_object(arr[i]))
            ob->remove() ;

        out += "> Loading channel module: " + arr[i] + "...";
        time = time_frac() ;
        err = catch(load_object(arr[i]));

        if(err != 0) {
            out += "\t\nError " + err + " when loading " + arr[i] + "";
        }

        else {
            out += sprintf(" Done (%.2fs)", time_frac() - time) ;
        }

        debug_message(out) ;
    }
}

int register_module(string name, string path) {
    string *keys;

    keys = keys(modules);
    if(!name || !path) return 0;

     if(member_array(name, keys) != -1) {
        if(modules[name] == path) return 1;
        else return -1;
    }
    modules[name] = path;

    return 1;
}

int unregister_module(string module_name) {
    string *keys = keys(modules);

    if(!module_name || member_array(module_name, keys) == -1) return 0;

    if(modules[module_name] != file_name(previous_object())) return 0;

    foreach(mixed key, mixed value in channels) {
        if(value["module"] == module_name) map_delete(channels, key);
    }

    return 1;
}

int register_channel(string module_name, string channel_name) {
    string *keys;
    string new_name = channel_name;

    keys = keys(modules);
    if(member_array(module_name, keys) == -1) return -1;
    keys = keys(channels);

    if(modules[module_name] != file_name(previous_object())) return 0;

    if(member_array(channel_name, keys) != -1) {
        if(channels[channel_name]["module"] == module_name) {
            channels[channel_name]["listeners"] = ({});
            return 1;
        }
        else new_name = module_name[0..3] + channel_name;
    }
    channels[new_name] = (["module" : module_name, "real_name" : channel_name, "listeners" : ({})]);

    return 1;
}

int remove_channel(string channel_name) {
    string *keys;

    keys = keys(channels);
    if(member_array(channel_name, keys) == -1) return 0;
    map_delete(channels, channel_name);

    return 1;
}

int tune(string channel, string user, int flag) {
    string *keys;
    object mod_obj;

    keys = keys(channels);
    if(member_array(channel, keys) == -1) return 0;

    if(!valid_module(channels[channel]["module"])) {
        map_delete(channels, channel);
        return 0;
    }

    mod_obj = find_object(modules[channels[channel]["module"]]);

    if(!mod_obj->is_allowed(channels[channel]["real_name"], user, flag)) return 0;
    if(flag == 1 && member_array(user, channels[channel]["listeners"]) == -1)
        channels[channel]["listeners"] += ({user});
    if(flag == 0 && member_array(user, channels[channel]["listeners"]) != -1)
        channels[channel]["listeners"] -= ({user});

    return 1;
}

int valid_ch(string channel_name) {
    string *keys;

    keys = keys(channels);
    if(member_array(channel_name, keys) != -1) return 1;

    return 0;
}

int valid_module(string module_name) {
    string *keys;

    keys = keys(modules);
    if(member_array(module_name, keys) != -1) return 1;

    return 0;
}

int chat(string channel, string user, string msg) {
    object mod_obj;
    string *keys;

    keys = keys(channels);
    if(member_array(channel, keys) == -1) return 0;

    if(!valid_module(channels[channel]["module"])) {
        map_delete(channels, channel);
        return 0;
    }
    mod_obj = find_object(modules[channels[channel]["module"]]);

    if(member_array(user, channels[channel]["listeners"]) == -1) return 0;
    if(!mod_obj->is_allowed(channels[channel]["real_name"], user)) return 0;
    if(!msg) return(notify_fail("Syntax: <channel> <msg>\n"));
    if(mod_obj->rec_msg(channels[channel]["real_name"], user, msg)) return 1;
    else return 0;
}

int muddy_chat(mapping payload) {
    object mod_obj;
    string *keys;
    string channel, user, msg, source ;
    int echoed ;

    channel = payload["channel"];
    user = payload["talker"];
    source = payload["identifier"];
    msg = payload["message"];
    echoed = payload["echoed"];

    keys = keys(channels);
    if(member_array(channel, keys) == -1) return 0;
    _debug("We actually here?");

    if(!valid_module(channels[channel]["module"])) {
        map_delete(channels, channel);
        return 0;
    }

    mod_obj = find_object(modules[channels[channel]["module"]]);
_debug("mod_obj: " + mod_obj);
_debug("channels[channel][\"real_name\"]: " + channels[channel]["real_name"]);
_debug("function_exists(\"rec_muddy_msg\", mod_obj): " + function_exists("rec_muddy_msg", mod_obj));
    if(function_exists("rec_muddy_msg", mod_obj) == 0) return 0;

    if(mod_obj->rec_muddy_msg(channels[channel]["real_name"], user, msg, source, echoed)) return 1;
    else return 0;
}

string *get_channels(string module_name) {
    string *ret = ({}), *keys;
    int i;

    keys = keys(channels);

    if(module_name == "all") ret = keys;
    else {
        for(i = 0; i < sizeof(keys); i++)
        if(channels[keys[i]]["module"] == module_name) ret += ({keys[i]});
    }

    ret = filter_array(ret, "filter_listing", this_object());
    ret = sort_array(ret, 1);

    return ret;
}

string *get_tuned(string argument) {
    string *ret = ({});

    if(!argument) return ret;
    if(sizeof(channels[argument]["listeners"]) <= 0) return ret;
    ret = channels[argument]["listeners"];
    foreach(string name in ret)
        if(find_living(name))
            if(!interactive(find_living(name))) ret -= ({ name });
    return ret;
}

int filter_listing(string element) {
    object mod_obj;
    catch(mod_obj = load_object(modules[channels[element]["module"]])) ;
    if(!mod_obj) return 0;
    if(mod_obj->is_allowed(element, this_player()->query_proper_name())) return 1;
    return 0;
}

string *get_modules() {
    string *keys = keys(modules);

    return keys;
}

void rec_msg(string channel, string msg) {
    string *listeners;
    string *keys;
    int i;
    object ob;

    keys = keys(channels);
    if(member_array(channel, keys) == -1) return 0;

     if(!valid_module(channels[channel]["module"])) {
        map_delete(channels, channel);
        return 0;
    }

    listeners = channels[channel]["listeners"];
    listeners -= ({ 0 }) ;
    if(listeners) {
        for(i = 0; i < sizeof(listeners); i ++) {
            ob = find_living(listeners[i]);

            if(!objectp(ob)) {
                channels[channel]["listeners"] -= ({ listeners[i] });
                continue;
            }
            tell_object(ob, msg);
        }
    }
}
