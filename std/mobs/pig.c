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
    mixed d ;

    if(!args)
        return ;

    data = args[0] ;

    set_name(data["name"]) ;
    set_short(data["short"]) ;
    set_long(data["long"]) ;
    set_id(({ data["id"]..., "pig", })) ;
    set_level(data["level"]) ;
    if(!nullp(data["gender"])) {
        if(stringp(data))
            set_gender(data["gender"]) ;
        else if(pointerp(data["gender"]))
            set_gender(element_of(data["gender"])) ;
        else if(mapp(data["gender"]))
            set_gender(element_of_weighted(data["gender"])) ;
    }

    if(!nullp(data["damage"]))
        set_damage(data["damage"]) ;
    if(!nullp(data["weapon_name"]))
        set_weapon_name(data["weapon_name"]) ;
    if(!nullp(data["weapon_type"]))
        set_weapon_type(data["weapon_type"]) ;

    set_race("pig") ;
}
