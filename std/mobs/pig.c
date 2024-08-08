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
    if(nullp(data["level"]))
        set_level(1) ;
    else {
        if(intp(data["level"]))
            set_level(data["level"]) ;
        else if(pointerp(data["level"]))
            set_level(to_float(random(data["level"][1] - data["level"][0]) + data["level"][0])) ;
        else
            set_level(1) ;
    }
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

    add_loot(({
        "/obj/armor/torso/leather_jerkin",
        "/obj/clothing/black_leather_shoes",
        "/obj/clothing/cream-coloured_linen_tunic",
        "/obj/clothing/tan_leather_breeches",
    }), 25.0) ;

    add_loot("/obj/weapon/piercing/rusty_sword") ;

    add_coin("copper", 1, 100.0) ;
    add_coin("silver", 1, 50.0) ;
}
