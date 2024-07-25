/**
 * @file /std/user/damage.c
 * @description Damage object for livings
 *
 * @created 2024/07/25 - Gesslar
 * @last_modified 2024/07/25 - Gesslar
 *
 * @history
 * 2024/07/25 - Gesslar - Created
 */

#include <vitals.h>

float deliver_damage(object victim, float damage) {
    if(!victim)
        return 0;

    if(damage < 0)
        return 0;

    return victim->receive_damage(this_object(), damage) ;
}
float receive_damage(object attacker, float damage) {
    if(!attacker)
        return 0;

    if(damage < 0)
        return 0;

    add_hp(-damage) ;

    return query_hp() - damage ;
}
