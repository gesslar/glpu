/**
 * @file /obj/weapon/piercing/rusty_sword.c
 * @description A basic sword for stabbing things
 *
 * @created 2024-08-04 - Gesslar
 * @last_modified 2024-08-04 - Gesslar
 *
 * @history
 * 2024-08-04 - Gesslar - Created
 */

inherit STD_WEAPON ;

void setup() {
    set_id(({ "sword", "rusty sword" })) ;
    set_short("rusty sword") ;
    set_name("rusty sword") ;
    set_long("This rusty sword is a simple, straight blade with a single "
             "edge. The blade is pitted and worn, and the hilt is wrapped in "
             "cracked leather. Despite its appearance, the sword is still "
             "sharp and serviceable, and the blade is straight and true.") ;
    set_mass(50) ;
    set_hands(1) ;
    set_dc(1) ;
    set_damage_type("piercing") ;

    set_value(10) ;
}
