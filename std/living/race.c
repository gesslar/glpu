/**
 * @file /std/user/race.c
 * @description Race stuff
 *
 * @created 2024-07-25 - Gesslar
 * @last_modified 2024-07-25 - Gesslar
 *
 * @history
 * 2024-07-25 - Gesslar - Created
 */

#include <race.h>
#include <module.h>

private nosave string racial_bodies = DIR_STD_MODULES_MOBILE "race/" ;
private nomask nosave string _race ;
private string gender ;

public string set_race(string race) {
    object module ;

    if(!file_exists(racial_bodies+"/"+race+".c")) {
        _race = race ;
        return _race ;
    }

    if(get_module(race))
        error("Race body module has already been applied") ;

    module = add_module("race/"+race) ;

    if(!objectp(module))
        error("Failed to add race module.") ;

    _race = module->query_race() ;

    return _race ;
}

public string query_race() {
    return _race || module("race", "query_race") ;
}

public nomask void set_gender(string g) {
    gender = g ;
}

public nomask string query_gender() {
    return gender ;
}

public nomask void adjust_living() {
}

public nomask void living_adjust_attributes() {
}

public nomask void living_adjust_vitals() {
}
