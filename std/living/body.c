/**
 * @file /std/user/body.c
 * @description Body object that is shared by players and NPCs.
 *
 * @created 2024/07/29 - Gesslar
 * @last_modified 2024/07/29 - Gesslar
 *
 * @history
 * 2024/07/29 - Gesslar - Created
 */

#include <body.h>
#include <player.h>

inherit STD_CONTAINER ;
inherit STD_ITEM;

inherit __DIR__ "advancement" ;
inherit __DIR__ "alias" ;
inherit __DIR__ "combat" ;
inherit __DIR__ "ed" ;
inherit __DIR__ "module" ;
inherit __DIR__ "pager" ;
inherit __DIR__ "race" ;

inherit M_LOG ;

/* Global Variables */
string *path;
nosave string *command_history = ({});
private nosave object link;

/* Prototypes */

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

string *query_body_slots() {
    return copy(body_slots);
}

string *query_all_commands() {
    return commands() ;
}

void init_capacity() {
    set_max_capacity(1000) ;
    set_max_volume(500) ;

    ::init_capacity() ;
}

void die() {
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
    if(userp())  {
        object ghost = BODY_D->create_ghost(query_user(), this_object()) ;
        exec(ghost, this_object()) ;
        ghost->setup_body() ;
        ghost->set_hp(-1.0) ;
        ghost->set_sp(-1.0) ;
        ghost->set_mp(-1.0) ;
        query_user()->set_body(ghost) ;
        ghost->move(environment()) ;
    } else {
        ADVANCE_D->kill_xp(killed_by(), this_object()) ;
    }

    remove() ;
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

    if(objectp(query_user()))
        destruct(query_user());

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

void receive_message(string type, string msg) {
    tell(this_object(), msg) ;
}

/* Communication & Interaction functions */
void catch_tell(string message) {
    receive_message("tell", message);
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

int query_log_level() {
    if(!query_env("log_level")) return 0 ;

    return to_int(query_env("log_level")) ;
}
