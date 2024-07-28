/**
 * @file /std/user/vitals.c
 * @description Vitals for livings
 *
 * @created 2024/07/24 - Gesslar
 * @last_modified 2024/07/24 - Gesslar
 *
 * @history
 * 2024/07/24 - Gesslar - Created
 */

#include <gmcp.h>
#include <vitals.h>
#include <runtime_config.h>

void update_regen_interval() ;

private float hp, max_hp, sp, max_sp, mp, max_mp ;
private int dead = false ;
private nosave int tick ;
private nosave int regen_interval_pulses; // Number of pulses to trigger a regen

void init_vitals() {
    hp = hp || 100.0 ;
    max_hp = max_hp || 100.0 ;
    sp = sp || 100.0 ;
    max_sp = max_sp || 100.0 ;
    mp = mp || 100.0 ;
    max_mp = max_mp || 100.0 ;

    update_regen_interval(); // Calculate regen interval based on the product of HEART_PULSE and HEARTBEATS_TO_REGEN
}

float query_hp() { return hp ; }
float query_max_hp() { return max_hp ; }
float hp_ratio() { return percent_of(hp, max_hp) ; }

float query_sp() { return sp ; }
float query_max_sp() { return max_sp ; }
float sp_ratio() { return percent_of(sp, max_sp) ; }

float query_mp() { return mp ; }
float query_max_mp() { return max_mp ; }
float mp_ratio() { return percent_of(mp, max_mp) ; }

void set_hp(float x) { hp = x ; }
void set_max_hp(float x) { max_hp = x ; }

void set_sp(float x) { sp = x ; }
void set_max_sp(float x) { max_sp = x ; }

void set_mp(float x) { mp = x ; }
void set_max_mp(float x) { max_mp = x ; }

float add_hp(float x) {
    hp += x ;
    if (hp > max_hp)
        hp = max_hp ;

    if(hp <= 0) {
        hp = 0 ;
    }

    GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_VITALS, ([
        GMCP_LBL_CHAR_VITALS_HP: sprintf("%.2f", hp),
    ])) ;

    return hp ;
}

float add_sp(float x) {
    sp += x ;
    if (sp > max_sp)
        sp = max_sp ;

    GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_VITALS, ([
        GMCP_LBL_CHAR_VITALS_SP: sprintf("%.2f", sp),
    ])) ;
    return sp ;
}

float add_mp(float x) {
    mp += x ;
    if (mp > max_mp)
        mp = max_mp ;

    GMCP_D->send_gmcp(this_object(), GMCP_PKG_CHAR_VITALS, ([
        GMCP_LBL_CHAR_VITALS_MP: sprintf("%.2f", mp),
    ])) ;
    return mp ;
}

protected void heal_tick(int force: (: 0 :)) {
    if(++tick >= regen_interval_pulses  || force) {
        if(!force)
            tick = 0 ;

        if(hp < max_hp)
            add_hp(random_float(2)) ;
        if(sp < max_sp)
            add_sp(random_float(2)) ;
        if(mp < max_mp)
            add_mp(random_float(2)) ;
    }
}

int set_heart_rate(int x) {
    if(x < 5)
        x = 5 ;
    else if(x > 100)
        x = 100 ;

    set_heart_beat(x) ;

    return x ;
}

int adjust_heart_rate(int x) {
    return set_heart_rate(query_heart_beat() + x) ;
}

// This function calculates the number of pulses needed based on HEART_PULSE and HEARTBEATS_TO_REGEN
void update_regen_interval() {
    // Calculate the number of pulses for the regen interval
    regen_interval_pulses = to_int((mud_config("HEART_PULSE") * mud_config("HEARTBEATS_TO_REGEN")) / 1000.0); // Convert ms to seconds
    tick = 0;
}

// This function initializes the healing process
void initialize_healing(object character) {
    update_regen_interval(); // Calculate regen interval based on the product of HEART_PULSE and HEARTBEATS_TO_REGEN
}

int query_heart_rate() {
    return query_heart_beat() ;
}

int query_regen_duration() {
    return regen_interval_pulses ;
}

void restore() {
    hp = max_hp ;
    sp = max_sp ;
    mp = max_mp ;
}

int set_dead(int x) {
    dead = !!x ;
    return dead ;
}

float query_condition() {
    return percent(hp, max_hp) ;
}

string query_condition_string() {
    float ratio = query_condition() ;
    if (ratio <= 0.0)
        return "dead";
    else if (ratio <= 15.5)
        return "critical";
    else if (ratio <= 30.0)
        return "severely injured";
    else if (ratio <= 45.0)
        return "moderately injured";
    else if (ratio <= 60.0)
        return "injured";
    else if (ratio <= 75.0)
        return "hurt";
    else if (ratio <= 90.0)
        return "wounded";
    else if(ratio < 100.0)
        return "bruised and nicked";
    else
        return "healthy";
}

int is_dead() { return dead ; }
