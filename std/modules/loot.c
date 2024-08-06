/**
 * @file /std/modules/loot.c
 * @description Loot module for anything.
 *
 * @created 2024/08/05 - Gesslar
 * @last_modified 2024/08/05 - Gesslar
 *
 * @history
 * 2024/08/05 - Gesslar - Created
 */

#include <type.h>

private nosave mixed *loot_table = ({ }) ;

varargs public void add_loot(mixed item, float chance) {
    if(nullp(item))
        return;

    if(nullp(chance))
        chance = 100.0 ;
    else if(intp(chance))
        chance = to_float(chance) ;

    if(!floatp(chance))
        return;

    chance = range(0.0, 100.0, chance) ;

    switch(typeof(item)) {
        case T_STRING :
        case T_MAPPING :
        case T_FUNCTION :
        case T_ARRAY :
            loot_table += ({ ({ item, chance }) }) ;
            break ;
        default :
            return;
    }
}

public varargs void set_loot_table(mixed *table) {
    if(!pointerp(table))
        return;

    loot_table = table;
}

public mixed *query_loot_table() {
    return copy(loot_table) ;
}
