/**
 * @file /obj/container/backpack.c
 * @description A backpack object
 *
 * @created 2024-08-07 - Gesslar
 * @last_modified 2024-08-07 - Gesslar
 *
 * @history
 * 2024-08-07 - Gesslar - Created
 */

inherit STD_CLOTHING ;
inherit STD_CONTAINER ;

void setup() {
    set_id("backpack") ;
    set_adj("small");
    set_name("backpack");
    set_short("a small backpack");
    set_long("A backpack for carrying items.");
    set_mass(5);
    set_capacity(10);
}
