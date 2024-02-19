/* mob.c

 Tacitus @ LPUniversity
 11-JUNE-06
 Character Object

*/

/* This object represents a charachter in the mud.
 Another object that inherits this represents NPCs
 */

/* Last edited by Tacitus on October 4th, 2006 */

#include <config.h>
#include <driver/origin.h>
#include <logs.h>

inherit STD_OBJECT;

inherit __DIR__ "alias";
// inherit __DIR__ "tell" ;

/* Global Variables */

string *path;
nosave string *command_history = ({});
private object link;

/* Prototypes */

/* Connection functions */

void enter_world();
void exit_world();
void net_dead();
void reconnect();

/* User object functions */

varargs int move(mixed ob, int flag);
void restore_user();
void save_user();
object query_link();
int set_link(object ob);

/* User environmental variable functions */

int set_env(string var_name, string var_value);
mixed query_env(string var_name);
mapping list_env();

/* User path functions */

string *query_path();
void add_path(string str);
void rem_path(string str);

/* Communication & Interaction functions */

void catch_tell(string message);
void receive_message(string type, string msg);
string process_input(string arg);
int command_hook(string arg);
mixed* query_commands();
int force_me(string cmd);

/* Misc functions */

void write_prompt();

/* Functions */

void create() {
    if(origin() != ORIGIN_DRIVER && origin() != ORIGIN_LOCAL) return;
    path = ({"/cmds/std/"});
}

/* Connection functions */

void setup() {
    add_action("command_hook", "", 1);
    set_living_name(name());
    set_ids(({name()}));
    set_proper_name(name());
    set_heart_beat(1);
    enable_commands();
    set("prevent_get", 1);
    if(!query("cwd")) set("cwd", "/doc");
    if(!query_long()) set_long(capitalize(name()) + "");
    if(!mapp(query("env_settings"))) set("env_settings", (["colour" : "enabled"]));
    if(!query_env("news_client")) set_env("news_client", "/obj/mudlib/newsclients/std_newsclient.c");
    if(!query_env("auto_tune")) set_env("auto_tune", "all");
    if(!query_env("biff")) set_env("biff", "on");
    if(!query_env("prompt")) set_env("prompt", ">");
}

void enter_world() {
    string *cmds, *ch;
    int i;
    object news_client, mail_client;

    if(!is_member(query_privs(previous_object()), "admin")) return;

    ANNOUNCE_CHDMOD->announce_login(name());

    catch {
        news_client = new(query_env("news_client"));
        news_client->move(this_object());
        if(news_client->is_new()) write("\nNotice: There are new news posts.\n\n");
        news_client->remove() ;
        mail_client = new(OBJ_MAIL_CLIENT);
        mail_client->move(this_object());
        if(mail_client->has_new_mail()) write("You have new mail!\n\n");
        mail_client->remove() ;
    };

    catch {
        ch = explode(query_env("auto_tune"), " ");
        if(sizeof(ch) > 0)
            foreach(string channel in ch)
               force_me("channel tune in " + channel);
    };

    if(file_size(user_path(name()) + ".login") > 0) {
        write("\n");
        cmds = explode(read_file(user_path(name()) + ".login"), "\n");
        if(sizeof(cmds) <= 0) return;
        for(i = 0; i < sizeof(cmds); i ++) catch(command(cmds[i]));
    }

    set("last_login", time());
    write("\n");
    say(capitalize(name()) + " has entered.\n");
}

void exit_world() {
    string *cmds;
    int i;

    if(this_player() != this_object()) return;

    if(file_size(user_path(name()) + ".quit") > 0) {
        cmds = explode(read_file(user_path(name()) + ".quit"), "\n");
        if(sizeof(cmds) <= 0) return;
        for(i = 0; i < sizeof(cmds); i ++) catch(command(cmds[i]));
    }

    set("last_login", time());

    if(environment())
        say((string)capitalize(name())+ " leaves " + mud_name() + ".\n");

    ANNOUNCE_CHDMOD->announce_logoff(name());

    save_user();
}

void net_dead() {
    if(origin() != ORIGIN_DRIVER) return;
    set("last_login", time());
    save_user();
    if(environment()) tell_room(environment(), capitalize(name()) + " has gone link-dead.\n");
    set_short(capitalize(name()) + " [link dead]");
    log_file(LOG_LOGIN, capitalize(name()) + " went link-dead on " + ctime(time()) + "\n");
}

void reconnect() {
    restore_user();
    set("last_login", time());
    write("Success: Reconnected.\n");
    if(environment()) tell_room(environment(), capitalize(name()) + " has reconnected.\n", this_player());
    set_short(capitalize(name()));
    /* reconnection logged in login object */
}

/* User Object Functions */

void heart_beat() {
    if(!interactive(this_object())) {
        if((time() - query("last_login")) > 3600) {
            if(environment())
                tell_room(environment(), capitalize(name()) + " fades out of existance.\n");
            log_file(LOG_LOGIN, capitalize(name()) + " auto-quit after 1 hour of net-dead at " + ctime(time()) + ".\n");
            destruct(this_object());
        }
    } else {
        /* Prevent link-dead from idle */
        if(query_idle(this_object()) % 60 == 0 && query_idle(this_object()) > 300
                && query_env("keepalive") && query_env("keepalive") != "off") {
            telnet_nop() ;
        }
    }
}

void restore_user() {
    if(!is_member(query_privs(previous_object() ? previous_object() : this_player()), "admin") && this_player() != this_object()) return 0;
    if(is_member(query_privs(previous_object()), "admin") || query_privs(previous_object()) == this_player()->name()) restore_object(user_mob_data(name()));
}

void save_user() {
    if(!is_member(query_privs(previous_object() ? previous_object() : this_player()), "admin") && this_player() != this_object()) return 0;
    catch(save_object(user_mob_data(name())));
}

varargs int move(mixed ob, int flag) {
    if(!::move(ob)) return 0;
    set("last_location", base_name(ob));
    if(!flag) command("look");
    return 1;
}

void event_remove(object prev) {
    if(objectp(query_link())) destruct(query_link());
}

object query_link() {
    return link;
}

int set_link(object ob) {
    if(query_privs(previous_object()) != query_privs(this_object())
        && !adminp(previous_object())) return 0;
    link = ob;
    return 1;
}

/* Environmental Settings */
int set_env(string var_name, string var_value) {
    mapping data = query("env_settings");
    if(!var_value) map_delete(data, var_name);
    else data += ([var_name : var_value]);
    set("env_settings", data);
    return 1;
}

mixed query_env(string var_name) {
    mapping data = query("env_settings");
    if(data[var_name]) return data[var_name];
}

mapping list_env() {
    mapping data = query("env_settings");
    return data;
}


/* User path functions */

string *query_path() {
    return copy(path);
}

void add_path(string str) {
    if(!adminp(previous_object()) && this_player() != this_object()) return;

    if(member_array(str, path) != -1) {
        write("Error [path]: Directory '" + str + "' is already in your path.\n");
        return;
    }

    if(str[<1] != '/') str += "/";

    if(!directory_exists(str)) {
        write("Error [path]: Directory '" + str + "' does not exist.\n");
        return;
    }

    path += ({str});
}

void rem_path(string str) {
    if(!adminp(previous_object()) && this_player() != this_object()) return;

    if(member_array(str, path) == -1) {
        write("Error [path]: Directory '" + str + "' is not in your path.\n");
        return;
    }

    path -= ({str});
}

/* Communication & Interaction functions */
void catch_tell(string message) {
    receive_message("unknown", message);
}

void receive_message(string type, string msg) {
    tell(this_object(), msg) ;
}

string process_input(string arg) {
    return arg ;
}

nomask varargs string *query_command_history(int index, int range) {
    if(this_player() != this_object() && !adminp(previous_object())) return ({});
    if(!index) return command_history + ({});
    else if(range) return command_history[index..range] + ({});
    else return ({ command_history[index] });
}

int command_hook(string arg) {
    string verb, err, *cmds = ({});
    string custom, tmp;
    object caller, command;
    int i;

    caller = this_player() ;

    if(interactive(caller)) if(caller != this_object()) return 0;

    if(query_env("away")) {
        write("You return from away\n");
        set_env("away", 0);
        return 1;
    }

    verb = query_verb();

    if(sscanf(alias_parse(verb, arg), "%s %s", verb, arg) != 2)
        verb = alias_parse(verb, arg);
    if(arg == "") arg = 0;
    verb = lower_case(verb);

    if(arg) command_history += ({ verb + " " + arg });
    else command_history += ({ verb });

    if(environment() && environment()->valid_exit(verb)) {
        arg = verb ;
        verb = "go" ;
    }

    catch {
        if(environment()) {
            // if(environment()->valid_exit(verb)) {
            //     if(this_player()->move_allowed(environment(this_player())->query_exit(verb))) {
            //         if(this_player()->query_env("move_in") && wizardp(this_player())) {
            //             custom = this_player()->query_env("move_in");
            //             tmp = custom;
            //             tmp = replace_string(tmp, "$N", query_cap_name());
            //             tell_room(environment(this_player())->query_exit(verb), capitalize(tmp) + "\n", this_player());
            //         } else {
            //             tell_room(environment(this_player())->query_exit(verb), capitalize(name()) + " has entered the room.\n", this_player());
            //         }

            //         if(this_player()->query_env("move_out") && wizardp(this_player())) {
            //             custom = this_player()->query_env("move_out");
            //             tmp = custom;
            //             tmp = replace_string(tmp, "$N", query_cap_name());
            //             tmp = replace_string(tmp, "$D", verb);
            //             tell_room(environment(this_player()), capitalize(tmp) + "\n", this_player());
            //         } else {
            //             tell_room(environment(this_player()), capitalize(name())
            //                 + " leaves through the " + verb + " exit.\n", this_player());
            //         }

            //         write("You move to " + environment(this_player())->query_exit(verb)->query_short() +
            //             ".\n\n");

            //         this_player()->move(environment(this_player())->query_exit(verb));
            //         return 1;
            //     } else {
            //         write("Error [move]: Unable to move through that exit.\n");
            //         return 1;
            //     }
            // }

            if(SOUL_D->request_emote(verb, arg)) return 1;
        }

        err = catch(load_object(CHAN_D));
        if(!err) {
            if(CHAN_D->snd_msg(verb, name(), arg)) return 1;
        }
    };

    for(i = 0; i < sizeof(path); i ++) {
        if(file_exists(path[i] + verb + ".c"))
            cmds += ({ path[i] + verb });
    }

    if(sizeof(cmds) > 0) {
        mixed return_value;

        i = 0;
        while(return_value <= 0 && i < sizeof(cmds)) {
            err = catch(command = load_object(cmds[i]));

            if(err) {
                write("Error: Command " + verb + " non-functional.\n");
                write(err);
                i++;
                continue;
            }

            return_value = command->main(caller, environment(), arg);
            i++;

            if(return_value) {
                if(stringp(return_value)) {
                    if(!strlen(return_value)) {
                        return_value = 0 ;
                    } else {
                        if(return_value[<1] != '\n') return_value += "\n" ;
                        message("info", return_value, this_object()) ;
                        return 1 ;
                    }
                } else if(pointerp(return_value)) {
                    if(!sizeof(return_value)) {
                        return_value = 0 ;
                    } else {
                        object pager = new(OBJ_PAGER) ;
                        pager->page(implode(return_value, "\n")) ;
                        return 1 ;
                    }
                }
            }
        }

        return return_value;
    }

    return 0;
}

varargs int move_living(mixed dest, string dir, string depart_message, string arrive_message) {
    int result ;
    object curr = environment() ;

    result = move(dest);

    if(result == MOVE_OK) { // Success
        string tmp ;

        if(curr) {
            if(!depart_message) depart_message = query_env("move_out");
            if(!depart_message) depart_message = "$N leaves $D.";
            if(!dir) dir = "somewhere" ;

            tmp = replace_string(depart_message, "$N", query_cap_name());
            tmp = replace_string(tmp, "$D", dir);

            tell_down(curr, tmp) ;
        }

        curr = environment() ;

        if(!arrive_message) arrive_message = query_env("move_in");
        if(!arrive_message) arrive_message = "$N arrives.";
        tmp = replace_string(arrive_message, "$N", query_cap_name());

        tell_down(curr, tmp, UNDEFINED, ({ this_object() })) ;
    } else {
        tell(this_object(), "You went nowhere.") ;
    }

    defer((: force_me, "look" :)) ;

    return result ;
}

mixed* query_commands() {
    return commands();
}

int force_me(string cmd) {
    if(!is_member(query_privs(previous_object()), "admin"))
        return 0;
    else
        return command(cmd);
}

//Misc functions
void write_prompt() {
    string prompt = query_env("prompt");

    write(prompt + " ");
}
