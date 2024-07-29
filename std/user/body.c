/* mob.c

 Tacitus @ LPUniversity
 11-JUNE-06
 Character Object

*/

/* This object represents a character in the mud.
 Another object that inherits this represents NPCs
 */

/* Last edited by Tacitus on October 4th, 2006 */

#include <origin.h>
#include <logs.h>
#include <rooms.h>
#include <classes.h>
#include <commands.h>
#include <gmcp.h>

inherit STD_ITEM;
inherit STD_CONTAINER ;

inherit __DIR__ "advancement" ;
inherit __DIR__ "alias" ;
inherit __DIR__ "combat" ;
inherit __DIR__ "ed" ;
inherit __DIR__ "pager" ;
inherit __DIR__ "race" ;
inherit __DIR__ "visibility" ;
inherit __DIR__ "vitals" ;
inherit __DIR__ "wealth" ;

inherit CLASS_GMCP ;

inherit M_ACTION ;
inherit M_GMCP ;
inherit M_LOG ;

/* Global Variables */
string *path;
nosave string *command_history = ({});
private nosave object link;

/* Prototypes */

/* Connection functions */

void enter_world();
void exit_world();
void reconnect();

/* User object functions */
varargs int move(mixed ob, int flag);
void restore_user();
void save_user();
object query_user();
int set_user(object ob);

/* User environmental variable functions */

int set_env(string var_name, string var_value);
varargs mixed query_env(string var_name, mixed def) ;
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
private nomask int evaluate_result(mixed result) ;
mixed* query_commands();
int force_me(string cmd);

// Module functions
void add_module(string module);
void remove_module(string module);
object get_module(string module);

// Death
void die() ;

private nosave mapping modules = ([]);
private nosave int finished_setup = 0 ;
private nosave mapping gmcp_data = ([ ]);
private nosave mapping environ_data = ([]) ;

/* Misc functions */

void write_prompt();

/* Functions */

void mudlib_setup() {
    // if(origin() != ORIGIN_DRIVER && origin() != ORIGIN_LOCAL) return;
    path = ({"/cmds/std/"});
    if(!query("env_settings"))
        set("env_settings", ([])) ;
    set_log_level(0) ;
}

private nosave string *body_slots = ({
    "head", "neck", "torso", "back", "arms", "hands", "legs", "feet"
});

void query_body_slots() {
    return copy(body_slots);
}

/* Connection functions */
void setup_body() {
    if(finished_setup)
        return;

    add_action("command_hook", "", 1);
    set_living_name(query_proper_name());
    set_ids(({query_proper_name()}));
    // set_proper_name(name());
    set_heart_beat(mud_config("DEFAULT_HEART_RATE")) ;
    if(!query_race()) set_race(mud_config("DEFAULT_RACE")) ;
    if(!query_level()) set_level(1.0) ;
    set_level_mod(0.0) ;
    enable_commands();
    set("prevent_get", 1);
    if(!query_env("cwd")) set_env("cwd", "/doc");
    if(!query_short()) set_short(query_name());
    if(!mapp(query("env_settings"))) set("env_settings", (["colour" : "on"]));
    if(!query_env("auto_tune")) set_env("auto_tune", "all");
    if(!query_env("biff")) set_env("biff", "on");
    if(!query_env("prompt")) set_env("prompt", ">");
    init_capacity() ;
    update_regen_interval() ;
    init_vitals() ;

    set_log_prefix(sprintf("(%O)", this_object())) ;

    slot(SIG_SYS_CRASH, "on_crash") ;
    slot(SIG_PLAYER_ADVANCED, "on_advance") ;
}

string *query_all_commands() {
    return commands() ;
}

void init_capacity() {
    set_max_capacity(1000) ;
    set_max_volume(500) ;

    ::init_capacity() ;
}

void enter_world() {
    string *cmds, *ch;
    int i;
    object news_client, mail_client;

    if(!is_member(query_privs(previous_object()), "admin"))
        return;

    catch {
        ch = explode(query_env("auto_tune"), " ");
        if(sizeof(ch) > 0)
            foreach(string channel in ch) {
                CMD_CHANNEL->tune(channel, query_privs(this_object()), 1, 1) ;
            }
    };

    set("last_login", time());
    write("\n");
    say(capitalize(query_name()) + " has entered.\n");
}

void exit_world() {
    string *cmds;
    int i;

    if(this_body() != this_object()) return;

    if(file_size(user_path(query_proper_name()) + ".quit") > 0) {
        cmds = explode(read_file(user_path(query_proper_name()) + ".quit"), "\n");
        if(sizeof(cmds) <= 0) return;
        for(i = 0; i < sizeof(cmds); i ++) catch(command(cmds[i]));
    }

    set("last_login", time());

    if(environment())
        say(query_name()+ " leaves " + mud_name() + ".\n");

    save_user();
}

void net_dead() {
    if(origin() != ORIGIN_DRIVER)
        return;

    abort_edit() ;

    set("last_login", time());
    save_user();

    if(environment())
        tell_all(environment(), query_name()+ " falls into stupour.\n");
    set("extra_short/link_dead", "[stupour]") ;
    log_file(LOG_LOGIN, query_proper_name() + " went link-dead on " + ctime(time()) + "\n");

    if(interactive(this_object()))
        emit(SIG_USER_LINKDEAD, this_object()) ;
}

void reconnect() {
    restore_user();
    set("last_login", time());
    tell(this_object(), "Success: Reconnected.\n");
    if(environment()) tell_room(environment(), query_name() + " has reconnected.\n", this_body());
    delete("extra_short/link_dead") ;
    /* reconnection logged in login object */
}

/* Body Object Functions */
void heart_beat() {
    if(userp()) {
        if(!interactive(this_object())) {
            if((time() - query("last_login")) > 3600) {
                if(environment())
                    tell_room(environment(), query_name() + " fades out of existance.\n");
                log_file(LOG_LOGIN, query_proper_name() + " auto-quit after 1 hour of net-dead at " + ctime(time()) + ".\n");
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

    if(!is_dead() && query_hp() <= 0.0) {
        set_dead(1) ;
        die() ;
        return ;
    }

    heal_tick() ;

    evaluate_heart_beat() ;

    if(gmcp_enabled())
        GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_VITALS, null) ;
}

void die() {
    object ghost ;
    object corpse ;

    if(!environment())
        return ;

    if(!is_dead())
        return ;

    stop_all_attacks() ;
    simple_action("$N $vhave perished.") ;

    save_user() ;
    emit(SIG_PLAYER_DIED, this_object(), killed_by()) ;
    corpse = new(OBJ_CORPSE) ;
    corpse->setup_corpse(this_object(), killed_by()) ;
    corpse->move(environment()) ;
    if(userp()) {
        ghost = BODY_D->create_ghost(query_user(), this_object()) ;
        exec(ghost, this_object()) ;
        ghost->setup_body() ;
        query_user()->set_body(ghost) ;
        ghost->move(environment()) ;
    } else {
        ADVANCE_D->kill_xp(killed_by(), this_object()) ;
    }

    remove() ;
}

void restore_user() {
    if(!is_member(query_privs(previous_object() ? previous_object() : this_body()), "admin") && this_body() != this_object()) return 0;
    if(is_member(query_privs(previous_object()), "admin") || query_privs(previous_object()) == this_body()->query_proper_name()) restore_object(user_mob_data(query_proper_name()));
}

int save_user() {
    int result ;

    if(!is_member(query_privs(previous_object() ? previous_object() : this_body()), "admin") && this_body() != this_object()) return 0;
    catch(result = save_object(user_mob_data(query_proper_name())));

    return result;
}

varargs int move(mixed ob, int flag) {
    int result = ::move(ob) ;

    if(!(result & MOVE_OK))
        return result ;

    set("last_location", base_name(ob));

    return result ;
}

void event_remove(object prev) {
    object *all ;

    if(objectp(query_user())) destruct(query_user());

    all = all_inventory() ;
    foreach(object ob in all) {
        if(ob->prevent_drop()) {
            ob->remove() ;
        } else {
            if(environment()) {
                int result = ob->move(environment()) ;
                if(result & MOVE_OK) {
                    if(!(result & MOVE_DESTRUCTED)) {
                        ob->remove() ;
                    }
                }
            }
        }
    }
}

object query_user() {
    return link;
}

int set_user(object ob) {
    if(query_privs(previous_object()) != query_privs()
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

varargs mixed query_env(string var_name, mixed def) {
    mapping data = query("env_settings") ;
    if(data[var_name]) return data[var_name] ;
    else return def ;
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
    if(!adminp(previous_object()) && this_body() != this_object()) return;

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
    if(!adminp(previous_object()) && this_body() != this_object()) return;

    if(member_array(str, path) == -1) {
        write("Error [path]: Directory '" + str + "' is not in your path.\n");
        return;
    }

    path -= ({str});
}

/* Communication & Interaction functions */
void catch_tell(string message) {
    receive_message("tell", message);
}

void receive_message(string type, string msg) {
    tell(this_object(), msg) ;
}

string process_input(string arg) {
    return arg ;
}

nomask varargs string *query_command_history(int index, int range) {
    if(this_body() != this_object() && !adminp(previous_object())) return ({});
    if(!index) return command_history + ({});
    else if(range) return command_history[index..range] + ({});
    else return ({ command_history[index] });
}

int command_hook(string arg) {
    string verb, err, *cmds = ({});
    string custom, tmp;
    object caller, command;
    int i;
    mixed result ;
    object *obs, ob ;

    caller = this_body() ;

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
    // First let's check in our immediate inventory
    obs = all_inventory() ;
    foreach(ob in obs) {
        result = ob->evaluate_command(this_object(), verb, arg) ;
        result = evaluate_result(result) ;
        if(result == 1) return 1 ;
    }
    // Now let's check in our environment
    if(environment()) {
        obs = ({ environment() }) + all_inventory(environment()) - ({ this_object() }) ;
        foreach(ob in obs) {
            result = ob->evaluate_command(this_object(), verb, arg) ;
            result = evaluate_result(result) ;
            if(result == 1) return 1 ;
        }
    }

    if(arg) command_history += ({ verb + " " + arg });
    else command_history += ({ verb });

    if(environment() && environment()->valid_exit(verb)) {
        arg = verb ;
        verb = "go" ;
    }

    catch {
        if(environment()) {
            // if(environment()->valid_exit(verb)) {
            //     if(this_body()->move_allowed(environment(this_body())->query_exit(verb))) {
            //         if(this_body()->query_env("move_in") && wizardp(this_body())) {
            //             custom = this_body()->query_env("move_in");
            //             tmp = custom;
            //             tmp = replace_string(tmp, "$N", query_cap_name());
            //             tell_room(environment(this_body())->query_exit(verb), capitalize(tmp) + "\n", this_body());
            //         } else {
            //             tell_room(environment(this_body())->query_exit(verb), capitalize(name()) + " has entered the room.\n", this_body());
            //         }

            //         if(this_body()->query_env("move_out") && wizardp(this_body())) {
            //             custom = this_body()->query_env("move_out");
            //             tmp = custom;
            //             tmp = replace_string(tmp, "$N", query_ssssssssssssssssssssse());
            //             tmp = replace_string(tmp, "$D", verb);
            //             tell_room(environment(this_body()), capitalize(tmp) + "\n", this_body());
            //         } else {
            //             tell_room(environment(this_body()), capitalize(name())
            //                 + " leaves through the " + verb + " exit.\n", this_body());
            //         }

            //         write("You move to " + environment(this_body())->query_exit(verb)->query_short() +
            //             ".\n\n");

            //         this_body()->move(environment(this_body())->query_exit(verb));
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
            if(CHAN_D->chat(verb, query_privs(), arg)) return 1;
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

            return_value = command->main(caller, arg);
            i++;
            result = evaluate_result(return_value) ;
            if(result == 1) return 1 ;
        }

        return return_value;
    }

    return 0;
}

private nomask int evaluate_result(mixed result) {
    if(stringp(result)) {
        if(!strlen(result)) {
            return 0 ;
        } else {
            result = append(result, "\n") ;
            page(result) ;
            return 1 ;
        }
    } else if(pointerp(result)) {
        if(!sizeof(result)) {
            return 0 ;
        } else {
            page(result) ;
            return 1 ;
        }
    }

    return result ;
}

varargs int move_living(mixed dest, string dir, string depart_message, string arrive_message) {
    int result ;
    object curr = environment() ;

    result = move(dest);
    if(result & MOVE_ALREADY_THERE)
        return result ;

    if(result & MOVE_OK) { // Success
        string tmp ;

        if(curr) {
            if(depart_message != "SILENT") {
                if(!depart_message) depart_message = query_env("move_out");
                if(!depart_message) depart_message = "$N leaves $D.";
                if(!dir) dir = "somewhere" ;

                tmp = replace_string(depart_message, "$N", query_name()) ;
                tmp = replace_string(tmp, "$D", dir);

                tmp = append(tmp, "\n") ;

                tell_down(curr, tmp) ;
            }
        }

        if(arrive_message != "SILENT") {
            curr = environment() ;

            if(!arrive_message) arrive_message = query_env("move_in");
            if(!arrive_message) arrive_message = "$N arrives.\n";
            tmp = replace_string(arrive_message, "$N", query_name());

            tell_down(curr, tmp, null, ({ this_object() })) ;
        }

        force_me("look") ;
    }

    return result ;
}

mixed* query_commands() {
    return commands();
}

int force_me(string cmd) {
    if(
            this_body() != this_object()
        && !adminp(previous_object())
        && !adminp(this_caller()))
        return 0;
    else
        return command(cmd);
}

//Misc functions
void write_prompt() {
    string prompt = query_env("prompt");

    tell(this_object(), prompt + " ") ;
}

varargs object add_module(string module, mixed args...) {
    object ob ;
    string path ;
    string name ;

    if(!module || module == "") error("Error [add_module]: Invalid module name.\n") ;
    if(modules[module]) error("Error [add_module]: Module " + module + " already exists.\n") ;

    path = "/std/modules/mobile/" + module + ".c" ;
    path = replace_string(path, " ", "_") ;
    if(!file_exists(path))
        error("Error [add_module]: Module " + module + " does not exist.\n") ;

    catch(ob = new(path)) ;
    if(!ob)
        error("Error [add_module]: Module " + module + " failed to load.\n") ;

    if(ob->attach(this_object(), args...) == 0) {
        ob->remove() ;
        return ;
    }

    name = ob->query_name() ;

    modules[name] = ob ;

    return ob ;
}

object query_module(string module) {
    if(!module || module == "")
        error("Error [query_module]: Invalid module name. " + module) ;
    if(!modules[module])
        return 0 ;

    return modules[module] ;
}

void remove_module(string module) {
    object ob ;

    if(!module || module == "") error("Error [remove_module]: Invalid module name.\n") ;
    if(!modules[module]) error("Error [remove_module]: Module " + module + " does not exist.\n") ;

    ob = modules[module] ;
    if(!objectp(ob))
        return ;

    ob->remove() ;
    map_delete(modules, module) ;
}

object get_module(string module) {
    object ob ;

    if(!module || module == "") error("Error [get_module]: Invalid module name.\n") ;
    ob = modules[module] ;

    if(!objectp(ob))
        return 0 ;

    return modules[module] ;
}

mapping query_modules() {
    return copy(modules) ;
}

varargs mixed module(string module, string func, mixed args...) {
    object ob ;

    if(!stringp(module) || module == "") error("Error [query_module]: Invalid module name.\n") ;
    if(!stringp(func) || func == "") error("Error [query_module]: Invalid function name.\n") ;

    ob = modules[module] ;

    if(!objectp(ob))
        return null ;

    return call_if(ob, func, args...) ;
}

void remove_all_modules() {
    foreach(string module, object ob in modules) {
        catch(ob->remove()) ;
    }

    modules = ([ ]) ;
}

protected nosave mapping equipment = ([ ]) ;
public mapping query_equipped() { return copy(equipment); }
public object equipped_on(string slot) { return equipment[slot] || null ; }
mixed can_equip(string slot, object ob) ;

int equip(string slot, object ob) {
    if(!of(slot, body_slots))
        return 0 ;

    if(equipment[slot])
        return 0 ;

    equipment[slot] = ob ;

    return 1 ;
}

int unequip(string slot) {
    if(!equipment[slot])
        return 0 ;

    map_delete(equipment, slot) ;

    return 1 ;
}

mixed can_equip(string slot, object ob) {
    if(!of(slot, body_slots))
        return "Your body cannot wear something in there." ;

    if(equipment[slot])
        return "You are already wearing something like that." ;

    return 1 ;
}

/**
 * @description This function is called by the driver when the environment of
 *              an object is destructed. It will attempt to move the object to
 *              the void or hall if possible. If it cannot move the object, it
 *              will attempt to save the object and then destruct it.
 * @param {object} ob - The environment of the object that was destructed.
 */
void move_or_destruct(object ob) {
    object env = environment() ;
    object new_env ;

    // If we didn't come from the driver, we don't need to do anything
    if(origin() != ORIGIN_DRIVER)
        return ;

    // Were we in the VOID? If not, temporarily set the new destination
    // to the VOID.
    if(!ob && env != find_object(VOID_ROOM))
        catch(ob = load_object(VOID_ROOM)) ;

    // Were we in the starting room? If not, temporarily set the new
    // destination to the starting room.
    if(!ob && env != find_object(ROOM_START))
        catch(ob = load_object(ROOM_START)) ;

    // If we still don't have a destination, we need to clone a copy of a
    // safe room that doesn't have anything in it, but will allow us to
    // move the object to it. Which just happens to be the void. So, let's get
    // a copy of the void.
    if(!ob) {
        /* This is bad.  Try to save them anyway. */
        catch(ob = new(VOID_ROOM)) ;
        if(!ob)
            return ;

        // We don't want this object persisting.
        ob->set_no_clean(0) ;
    }

    move_object(ob) ;
}

void clear_gmcp_data() {
    gmcp_data = ([ ]) ;
}

void set_gmcp_client(mapping data) {
    gmcp_data["client"] = data;
}

mapping query_gmcp_client() {
    return copy(gmcp_data["client"]);
}

void set_gmcp_supports(mapping data) {
    gmcp_data["supports"] = data;
}

mapping query_gmcp_supports() {
    return copy(gmcp_data["supports"]);
}

// Function to determine if a specific package (and optionally module/
// submodule) is supported
int query_gmcp_supported(string fullname) {
    string *parts, package, module, submodule;
    mapping supports = query_gmcp_supports();
    class ClassGMCP gmcp ;

    gmcp = GMCP_D->convert_message(fullname) ;

    // Check if the package is supported
    if (!supports[package]) return 0; // Package not found

    // If a module is specified, check for its support
    if (module && supports[package]["modules"]) {
        if (!supports[package]["modules"][module]) return 0; // Module not found

        // If a submodule is specified, check for its support
        if (submodule && supports[package]["modules"][module]["submodules"]) {
            if (!supports[package]["modules"][module]["submodules"][submodule]) return 0; // Submodule not found
        } else if (submodule) {
            // Submodule specified but no submodules are supported under the module
            return 0;
        }
    } else if (module) {
        // Module specified but no modules are supported under the package
        return 0;
    }

    // If we've reached this point, the specified package (and optionally module/submodule) is supported
    return 1;
}

mixed query_environ(string key) {
    return environ_data[key] ;
}

void clear_environ_data() {
    environ_data = ([ ]) ;
}

mapping query_all_environ() {
    return copy(environ_data) ;
}

void set_environ_option(string key, mixed value) {
    environ_data[key] = from_string(value) ;

    _log(1, "Setting environ option: %s = %O", key, value) ;
}

void receive_environ(string var, mixed value) {
    set_environ_option(var, value) ;
}

void set_environ(mapping data) {
    if(base_name(previous_object()) != LOGIN_OB)
        return;

    if(!mapp(data))
        return ;

    foreach(string key, mixed value in data) {
        if(stringp(value))
            set_environ_option(key, value) ;
        else
            environ_data[key] = value ;
    }
}

int has_screenreader() {
    if(query_environ("SCREEN_READER") == true)
        return true ;

    if(query_env("screenreader") == "on")
        return true ;

    return false ;
}

int query_log_level() {
    if(!query_env("log_level")) return 0 ;

    return to_int(query_env("log_level")) ;
}

int supports_unicode() {
    if(has_screenreader()) return 0 ;
    return query_env("unicode") == "on" ;
}

void on_crash(mixed arg...) {
    int result ;

    if(previous_object() != signal_d())
        return ;

    catch(result = save_user()) ;
}

void mudlib_unsetup() {
    if(interactive())
        emit(SIG_USER_LOGOUT, this_object()) ;
}

int is_pc() { return 1 ; }
int is_npc() { return 0 ; }
