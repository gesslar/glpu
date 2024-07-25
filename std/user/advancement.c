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

float query_xp() {
    return random_float(100) ;
}

float query_tnl() {
    return 100.0 - random_float(100) ;
}
