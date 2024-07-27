/**
 * @file /std/user/race.c
 * @description Race stuff
 *
 * @created 2024/07/25 - Gesslar
 * @last_modified 2024/07/25 - Gesslar
 *
 * @history
 * 2024/07/25 - Gesslar - Created
 */

#include <body.h>

private nosave string racial_bodies = DIR_STD_MODULES_MOBILE "race/" ;
private nomask race ;

public string set_race(string race) {
    object module ;

    if(!file_exists(racial_bodies+"/"+race+".c"))
        error("No such body module for race: " + race) ;


    if(get_module(race))
        error("Race body module has already been applied") ;

    module = add_module("race/"+race) ;

    if(!objectp(module))
        error("Failed to add race module.") ;

    race = module->query_race() ;

    return race ;
}

public string query_race() {
    return module("race", "query_race") ;
}
