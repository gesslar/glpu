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

private float hp, max_hp, sp, max_sp, mp, max_mp ;
private int dead = false ;
private nosave int tick ;

void init_vitals() {
    hp = hp || 100.0 ;
    max_hp = max_hp || 100.0 ;
    sp = sp || 100.0 ;
    max_sp = max_sp || 100.0 ;
    mp = mp || 100.0 ;
    max_mp = max_mp || 100.0 ;

    update_heal_tick() ;
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
    if(++tick >= 10 || force) {
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

void update_heal_tick(int tth: (: mud_config("TIME_TO_HEAL") :)) {
    int ticks ;

    if(tth < 10)
        tth = 10 ;

    ticks = tth / mud_config("HEARTBEATS_TO_REGEN") ;
    tick = 0 ;
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

int is_dead() { return dead ; }
