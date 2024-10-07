/**
 * @file /obj/food/muffin.c
 * @description A muffin food
 *
 * @created 2024-08-06 - Gesslar
 * @last_modified 2024-08-06 - Gesslar
 *
 * @history
 * 2024-08-06 - Gesslar - Created
 */

inherit STD_FOOD ;

void setup() {
    set_id("muffin") ;
    set_name("muffin") ;
    set_short("a muffin") ;
    set_long("A delicious muffin.") ;
    set_mass(5) ;
    set_value(2) ;
    set_uses(1) ;
}
