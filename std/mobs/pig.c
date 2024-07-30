/**
 * @file /std/mobiles/pig.c
 * @description Virtual pigs
 *
 * @created 2024/07/27 - Gesslar
 * @last_modified 2024/07/27 - Gesslar
 *
 * @history
 * 2024/07/27 - Gesslar - Created
 */

inherit STD_NPC ;

void virtual_setup(mixed args...) {
    mapping data ;

    if(!args)
        return ;

    data = args[0] ;

    set_name(data["name"]) ;
    set_short(data["short"]) ;
    set_long(data["long"]) ;
    set_id(({ data["id"]..., "pig", })) ;
    set_level(data["level"]) ;
    set_race("human") ;
}
