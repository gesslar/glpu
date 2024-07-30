/**
 * @file /std/user/combat.c
 * @description Combat module. BANG! BOOM! POW!
 *
 * @created 2024/07/24 - Gesslar
 * @last_modified 2024/07/24 - Gesslar
 *
 * @history
 * 2024/07/24 - Gesslar - Created
 */

#include <combat.h>
#include <damage.h>
#include <gmcp_defines.h>
#include <vitals.h>
#include <action.h>
#include <module.h>
#include <equipment.h>

inherit __DIR__ "damage" ;

// Functions from other objects
string query_name() ;

// Variables
private nosave mapping _current_enemies = ([]);
private nosave mapping seen_enemies = ([]);
private nosave float attack_speed = 2.0;
private nosave int next_combat_round = 0;
private nosave mapping _defense = ([]);
private nosave float _ac = 0.0;
private nosave object last_damager ;
private nosave object killed_by_ob ;
private nosave string *combat_memory = ({ }) ;

void combat_round() {
    object enemy ;

    if(is_dead())
        return;

    if(query_hp() <= 0.0) {
        stop_all_attacks();
        return;
    }

    clean_up_enemies() ;

    if(!in_combat()) {
        next_combat_round = 0;
        return;
    }

    enemy = highest_threat();

    if(!valid_enemy(enemy))
        return;

    swing() ;

    GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_STATUS, ([
        GMCP_LBL_CHAR_STATUS_CURRENT_ENEMY: enemy->query_name(),
        GMCP_LBL_CHAR_STATUS_CURRENT_ENEMY_HEALTH: sprintf("%.2f", enemy->hp_ratio()),
        GMCP_LBL_CHAR_STATUS_CURRENT_ENEMIES: keys(_current_enemies),
    ])) ;

    if(find_call_out(next_combat_round) == -1)
        next_round() ;
}

int start_attack(object victim) {
    if(!victim)
        return 0;

    if(_current_enemies[victim])
        return 0;

    _current_enemies[victim] = 1.0 ;

    if(!seen_enemies[victim]) {
        seen_enemies[victim] = 1.0;
    }

    if(!userp())
        module("combat_memory", "add_to_memory", victim) ;

    next_combat_round = call_out_walltime("combat_round", attack_speed);

    victim->start_attack(this_object());

    return 1;
}

void swing() {
    object enemy = highest_threat();

    if(!enemy)
        return;

    if(!valid_enemy(enemy))
        return;

    if(!query_mp()) {
        tell(this_object(), "You are too exhausted to attack.\n");
        return;
    }

    if(!can_strike(enemy))
        return;

    strike_enemy(enemy);
}

int next_round() {
    float speed = attack_speed ;

    speed += random_float(1.5) ;

    next_combat_round = call_out_walltime("combat_round", speed);

    return next_combat_round;
}

private int can_strike(object enemy) {
    float ac = enemy->query_ac() ;
    float chance = mud_config("DEFAULT_HIT_CHANCE") ;
    float lvl = query_effective_level() ;
    float vlvl = enemy->query_effective_level() ;
    float result ;
    string name, vname ;
    object env ;
    object weapon ;
    string wname, wtype ;
    string *messes, mess ;

    chance = chance
           + (lvl - vlvl)
           - (ac * 2.0)
           ;

    result = random_float(100.0) ;

    if(result < chance)
        return 1;

    name = query_name() ;
    vname = enemy->query_name() ;

    env = environment() ;

    weapon = query_equipped()["weapon"] || null ;
    if(weapon) {
        wname = weapon->query_name() ;
        wtype = weapon->query_type() ;
    } else {
        wname = "fist" ;
        wtype = "bludgeoning" ;
    }

    mess = MESS_D->get_message("combat", wtype, 0) ;
    messes = ACTION_D->action(({ this_object(), enemy }), mess, ({wname})) ;

    tell(this_object(), messes[0], MSG_COMBAT_MISS) ;
    tell(enemy, messes[1], MSG_COMBAT_MISS) ;
    tell_down(env, messes[2], MSG_COMBAT_MISS, ({ this_object(), enemy })) ;

    return 0;
}

void strike_enemy(object enemy) {
    object weapon ;
    string wname, wtype ;
    string *messes, mess ;
    float dam ;

    if(!valid_enemy(enemy))
        return;

    if(!current_enemy(enemy))
        return;

    weapon = query_equipped()["weapon"] || null ;
    if(weapon) {
        wname = weapon->query_name() ;
        wtype = weapon->query_type() ;
    } else {
        wname = "fist" ;
        wtype = "bludgeoning" ;
    }

    dam = random_float(10.0) ;
    mess = MESS_D->get_message("combat", wtype, to_int(ceil(dam))) ;
    messes = ACTION_D->action(({ this_object(), enemy }), mess, ({wname})) ;

    tell(this_object(), messes[0], MSG_COMBAT_HIT) ;
    tell(enemy, messes[1], MSG_COMBAT_HIT) ;
    tell_down(environment(), messes[2], MSG_COMBAT_HIT, ({ this_object(), enemy })) ;

    deliver_damage(enemy, dam, "bludgeoning");
    add_mp(-random_float(5.0));
    add_threat(enemy, 1.0);
    add_seen_threat(enemy, 1.0);
}

int attacking(object victim) {
    if(!victim)
        return 0;

    if(_current_enemies[victim])
        return 1;

    return 0;
}

varargs int stop_attack(object victim, int seen) {
    if(!victim)
        return -1 ;

    if(!_current_enemies[victim])
        return -1 ;

    if(_current_enemies[victim]) {
        map_delete(_current_enemies, victim);
        return 1;
    }

    if(seen && seen_enemies[victim]) {
        map_delete(seen_enemies, victim);
        return 1;
    }

    return 0;
}

void stop_all_attacks() {
    if(find_call_out(next_combat_round) != -1)
        remove_call_out(next_combat_round);

    _current_enemies = ([]);

    GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_STATUS, ([
        GMCP_LBL_CHAR_STATUS_CURRENT_ENEMY: "",
        GMCP_LBL_CHAR_STATUS_CURRENT_ENEMIES: ({}),
    ])) ;

    clean_up_enemies();

    if(is_dead())
        return;
}

int in_combat() {
    return sizeof(_current_enemies) > 0;
}

int seen_enemy(object victim) {
    if(!victim)
        return 0;

    if(seen_enemies[victim])
        return 1;

    return 0;
}

int current_enemy(object victim) {
    if(!victim)
        return 0;

    if(_current_enemies[victim])
        return 1;

    return 0;
}

mapping current_enemies() {
    return copy(_current_enemies) ;
}

object highest_threat() {
    object *enemies = keys(_current_enemies);
    object highest = enemies[0];
    int highest_threat = 0;

    foreach(object enemy in enemies) {
        if(_current_enemies[enemy] > highest_threat) {
            highest = enemy;
            highest_threat = _current_enemies[enemy];
        }
    }

    return highest;
}

object lowest_threat() {
    object *enemies = keys(_current_enemies);
    object lowest = enemies[0];
    int lowest_threat = MAX_INT ;

    foreach(object enemy in enemies) {
        if(_current_enemies[enemy] < lowest_threat) {
            lowest = enemy;
            lowest_threat = _current_enemies[enemy];
        }
    }

    return lowest;
}

object highest_seen_threat() {
    object *enemies = keys(seen_enemies);
    object highest = enemies[0];
    int highest_threat = 0;

    foreach(object enemy in enemies) {
        if(seen_enemies[enemy] > highest_threat) {
            highest = enemy;
            highest_threat = seen_enemies[enemy];
        }
    }

    return highest;
}

object lowest_seen_threat() {
    object *enemies = keys(seen_enemies);
    object lowest = enemies[0];
    int lowest_threat = MAX_INT ;

    foreach(object enemy in enemies) {
        if(seen_enemies[enemy] < lowest_threat) {
            lowest = enemy;
            lowest_threat = seen_enemies[enemy];
        }
    }

    return lowest;
}

void clean_up_enemies() {
    if(is_dead())
        return;

    if(!in_combat())
        return;

    _current_enemies = filter(_current_enemies, (: valid_enemy :));

    if(!in_combat()) {
        next_combat_round = 0;

        if(query_hp() > 0.0)
            tell(this_object(), "You are no longer in combat.\n");

        GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_STATUS, ([
            GMCP_LBL_CHAR_STATUS_CURRENT_ENEMY: "",
            GMCP_LBL_CHAR_STATUS_CURRENT_ENEMIES: ({}),
        ])) ;
    }
}

varargs int valid_enemy(object enemy) {
    if(!same_env_check(this_object(), enemy))
        return 0 ;

    if(enemy->is_dead())
        return 0 ;

    return 1;
}

void clean_up_seen_enemies() {
    seen_enemies = filter(seen_enemies, (: valid_seen_enemy :));
}

varargs int valid_seen_enemy(object enemy, int threat) {
    if(!objectp(enemy))
        return 0 ;

    if(enemy->is_dead())
        return 0 ;

    return 1;
}

float add_threat(object enemy, float amount) {
    if(!valid_enemy(enemy))
        return 0.0;

    _current_enemies[enemy] += amount;

    return _current_enemies[enemy];
}

float add_seen_threat(object enemy, float amount) {
    if(!valid_seen_enemy(enemy))
        return 0.0;

    seen_enemies[enemy] += amount;

    return seen_enemies[enemy];
}

float add_attack_speed(float amount) {
    attack_speed += amount;

    attack_speed = range(0.5, 10.0, attack_speed);

    return attack_speed;
}

void set_attack_speed(float speed) {
    attack_speed = speed;
}

float query_attack_speed() {
    return attack_speed;
}

void set_defense(mapping def) {
    _defense = def ;
}

void add_defense(string type, float amount) {
    if(!_defense) _defense = ([ ]) ;
    _defense[type] = amount ;
}

mapping query_defense() {
    return copy(_defense) ;
}

float query_defense_amount(string type) {
    if(!_defense) return 0.0 ;
    return _defense[type] ;
}

mapping adjust_protection() {
    mapping equipment = query_equipped() ;
    object *obs = values(equipment), ob ;

    { // Defenses
        _defense = ([]);
        foreach(ob in obs) {
            mapping def = ob->query_defense() ;

            if(!mapp(def))
                continue ;

            foreach(string type, float amount in def) {
                if(!_defense[type])
                    _defense[type] = 0.0 ;

                _defense[type] += amount ;
            }
        }
    }

    { // Armor Class
        _ac = 0.0 ;
        foreach(ob in obs) {
            _ac += ob->query_ac() ;
        }
    }
}

object last_damaged_by() {
    return last_damager ;
}

object set_last_damaged_by(object ob) {
    last_damager = ob ;
    return last_damager ;
}

object killed_by() {
    return killed_by_ob ;
}

object set_killed_by(object ob) {
    killed_by_ob = ob ;
    return killed_by_ob ;
}
