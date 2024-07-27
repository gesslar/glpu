/**
 * @file /std/user/advancement.c
 * @description Advancement object for players
 *
 * @created 2024/07/24 - Gesslar
 * @last_modified 2024/07/24 - Gesslar
 *
 * @history
 * 2024/07/24 - Gesslar - Created
 */

#include <advancement.h>

private float level = 1.0 ;
private float xp = 0.0 ;
private float tnl = 100.0 ;

float query_xp() {
    return random_float(100) ;
}

float query_tnl() {
    return 100.0 - random_float(100) ;
}

float query_level() {
    return level ;
}

float set_level(float l) {
    level = to_float(l) ;
    tnl = 100.0 * level ;
    return level ;
}

float add_xp(float amount) {
    xp += to_float(amount) ;
    if(xp >= tnl) {
        xp -= tnl ;
        level++ ;
        tnl = 100.0 * level ;
    }
    return xp ;
}

float set_xp(float amount) {
    xp = to_float(amount) ;
    while(xp >= tnl) {
        xp -= tnl ;
        level++ ;
        tnl = 100.0 * level ;
    }
    return xp ;
}
