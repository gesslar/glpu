/**
 * @file /std/living/body.c
 * @description Body object that is shared by players and NPCs.
 *
 * @created 2024-07-29 - Gesslar
 * @last_modified 2024-07-29 - Gesslar
 *
 * @history
 * 2024-07-29 - Gesslar - Created
 */

#include <body.h>
#include <commands.h>
#include "/std/living/include/env.h"
#include <player.h>
#include <gmcp_defines.h>

inherit STD_CONTAINER ;
inherit STD_ITEM ;

inherit __DIR__ "act" ;
inherit __DIR__ "advancement" ;
inherit __DIR__ "alias" ;
inherit __DIR__ "appearance" ;
inherit __DIR__ "attributes" ;
inherit __DIR__ "boon" ;
inherit __DIR__ "combat" ;
inherit __DIR__ "communication" ;
inherit __DIR__ "ed" ;
inherit __DIR__ "env" ;
inherit __DIR__ "equipment" ;
inherit __DIR__ "module" ;
inherit __DIR__ "pager" ;
inherit __DIR__ "race" ;
inherit __DIR__ "skills" ;
inherit __DIR__ "visibility" ;
inherit __DIR__ "vitals" ;
inherit __DIR__ "wealth" ;

inherit M_ACTION ;
inherit M_LOG ;

/* Global Variables */
string *path ;
nosave string *command_history = ({}) ;
object su_body ;

/* Prototypes */

void mudlib_setup() {
    enable_commands() ;
    path = ({"/cmds/std/","/cmds/ability/", "/cmds/spell/"}) ;
    if(!query_pref("prompt")) set_pref("prompt", ">") ;
    set_log_level(0) ;
    set_prevent_get(1) ;
    add_action("command_hook", "", 1) ;
    set_ignore_mass(1) ;
}

private nosave string *body_slots = ({
    "head", "neck", "torso", "back", "arms", "hands", "legs", "feet"
}) ;

private nosave string *weapon_slots = ({
    "right hand", "left hand"
}) ;

string *query_body_slots() {
    return copy(body_slots) ;
}

string *query_weapon_slots() {
    return copy(weapon_slots) ;
}

string *query_all_commands() {
    return commands() ;
}

void rehash_capacity() {
    if(mud_config("USE_MASS") && !query_capacity())
        set_capacity(1000) ;

    ::rehash_capacity() ;
}

void die() {
    object corpse ;
    object ob, next ;

    if(!environment())
        return ;

    if(!is_dead())
        return ;

    stop_all_attacks() ;

    if(objectp(su_body)) {
        exec(su_body, this_object()) ;
        su_body->move(environment()) ;
        su_body->simple_action("$N $vis violently ejected from the body of $o.", this_object()) ;
        clear_su_body() ;
    }

    simple_action("$N $vhave perished.") ;
    save_body() ;
    emit(SIG_PLAYER_DIED, this_object(), killed_by()) ;
    corpse = new(OBJ_CORPSE) ;
    corpse->setup_corpse(this_object(), killed_by()) ;

    if(function_exists("query_loot_table"))
        LOOT_D->loot_drop(killed_by(), this_object()) ;
    if(function_exists("query_coin_table"))
        LOOT_D->coin_drop(killed_by(), this_object()) ;

    ob = first_inventory(this_object()) ;
    while(ob) {
        next = next_inventory(ob) ;
        if(ob->move(corpse))
            ob->remove() ;
        ob = next ;
    }

    // Now move coin objects to the corpse
    if(query_total_wealth()) {
        mapping wealth = query_all_wealth() ;
        foreach(string currency, int amount in wealth) {
            object coin = new(OBJ_COIN) ;
            coin->set_up(currency, amount) ;
            if(coin->move(corpse))
                if(coin)
                    coin->remove() ;
        }
    }

    if(corpse->move(environment()))
        corpse->remove() ;

    if(userp())  {
        object ghost = BODY_D->create_ghost(query_privs()) ;
        exec(ghost, this_object()) ;
        ghost->setup_body() ;
        ghost->set_hp(-1.0) ;
        ghost->set_sp(-1.0) ;
        ghost->set_mp(-1.0) ;
        ghost->move(environment()) ;
    } else {
        ADVANCE_D->kill_xp(killed_by(), this_object()) ;
    }

    remove() ;
}

varargs int move(mixed ob, int flag) {
    int result ;
    object env ;

    env = environment() ;
    result = ::move(ob) ;

    if(result)
        return result ;

    if(env)
        set_last_location(env) ;

    return result ;
}

void event_remove(object prev) {
    object *all ;

    all = all_inventory() ;
    foreach(object ob in all) {
        if(ob->prevent_drop()) {
            ob->remove() ;
        } else {
            if(environment()) {
                int result = ob->move(environment()) ;
                if(result) {
                    if(result == MOVE_DESTRUCTED) {
                        ob->remove() ;
                    }
                }
            }
        }
    }
}

/* User path functions */

string *query_path() {
    return copy(path) ;
}

int add_path(string str) {
    if(!adminp(previous_object()) && this_body() != this_object())
        return 0 ;

    if(member_array(str, path) != -1)
        return 0 ;

    str = append(str, "/") ;

    if(!directory_exists(str))
        return 0 ;

    path += ({str}) ;

    return 1 ;
}

int rem_path(string str) {
    if(!adminp(previous_object()) && this_body() != this_object())
        return 0 ;

    if(member_array(str, path) == -1)
        return 0 ;

    path -= ({str}) ;
    return 1 ;
}

void receive_message(string type, string msg) {
    do_receive(msg, DIRECT_MSG) ;
}

string process_input(string arg) {
    return arg ;
}

nomask varargs string *query_command_history(int index, int range) {
    if(this_body() != this_object() && !adminp(previous_object())) return ({}) ;
    if(!index) return command_history + ({}) ;
    else if(range) return command_history[index..range] + ({}) ;
    else return ({ command_history[index] }) ;
}

int command_hook(string arg) {
    string verb, err, *cmds = ({}) ;
    string custom, tmp ;
    object caller, command ;
    int i ;
    mixed result ;
    object *obs, ob ;

    caller = this_body() ;

    if(interactive(caller))
        if(caller != this_object())
            return 0 ;

    verb = query_verb() ;

    if(sscanf(alias_parse(verb, arg), "%s %s", verb, arg) != 2)
        verb = alias_parse(verb, arg) ;

    if(arg == "")
        arg = 0 ;

    verb = lower_case(verb) ;
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
            if(result == 1)
                return 1 ;
        }
    }

    if(arg) command_history += ({ verb + " " + arg }) ;
    else command_history += ({ verb }) ;

    if(environment() && environment()->valid_exit(verb)) {
        arg = verb ;
        verb = "go" ;
    }

    catch {
        if(environment()) {
            if(SOUL_D->request_emote(verb, arg)) return 1 ;
        }

        err = catch(load_object(CHAN_D)) ;
        if(!err) {
            if(CHAN_D->chat(verb, query_privs(), arg)) return 1 ;
        }
    } ;

    for(i = 0; i < sizeof(path); i ++) {
        if(file_exists(path[i] + verb + ".c"))
            cmds += ({ path[i] + verb }) ;
    }

    if(sizeof(cmds) > 0) {
        mixed return_value ;

        i = 0 ;
        while(return_value <= 0 && i < sizeof(cmds)) {
            err = catch(command = load_object(cmds[i])) ;

            if(err) {
                write("Error: Command " + verb + " non-functional.\n") ;
                write(err) ;
                i++ ;
                continue ;
            }

            return_value = command->main(caller, arg) ;
            i++ ;
            result = evaluate_result(return_value) ;
            if(result == 1) return 1 ;
        }

        return return_value ;
    }

    return 0 ;
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
    string tmp ;

    result = move(dest) ;
    if(result)
        return result ;

    if(is_acting()) {
        tell(this_object(), "You stop what you are doing.\n") ;
        cancel_acts() ;
    }

    if(curr) {
        if(depart_message != "SILENT") {
            if(!depart_message) depart_message = query_env("move_out") ;
            if(!depart_message) depart_message = "$N leaves $D." ;
            if(!dir) dir = "somewhere" ;

            tmp = replace_string(depart_message, "$N", query_name()) ;
            tmp = replace_string(tmp, "$D", dir) ;

            tmp = append(tmp, "\n") ;

            tell_down(curr, tmp) ;
        }
    }

    if(arrive_message != "SILENT") {
        curr = environment() ;

        if(!arrive_message) arrive_message = query_env("move_in") ;
        if(!arrive_message) arrive_message = "$N arrives.\n" ;
        tmp = replace_string(arrive_message, "$N", query_name()) ;

        tmp = append(tmp, "\n") ;

        tell_down(curr, tmp, null, ({ this_object() })) ;
    }

    force_me("look") ;

    GMCP_D->send_gmcp(this_object(), GMCP_PKG_ROOM_INFO, environment()) ;

    return result ;
}

mixed* query_commands() {
    return commands() ;
}

int force_me(string cmd) {
    if(
            this_body() != this_object()
        && !adminp(previous_object())
        && !adminp(this_caller()))
        return 0 ;
    else
        return command(cmd) ;
}

//Misc functions
void write_prompt() {
    string prompt = query_pref("prompt") ;

    receive(prompt + " ") ;
}

int query_log_level() {
    if(!query_pref("log_level")) return 0 ;

    return to_int(query_pref("log_level")) ;
}

void set_su_body(object source) {
    su_body = source ;
}

object query_su_body() {
    return su_body ;
}

void clear_su_body() {
    su_body = null ;
}
