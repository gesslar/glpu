/**
 * @file /obj/drink/strawberry_juice.c
 * @description A strawberry juice drink
 *
 * @created 2024-08-06 - Gesslar
 * @last_modified 2024-08-06 - Gesslar
 *
 * @history
 * 2024-08-06 - Gesslar - Created
 */

inherit STD_DRINK ;

void setup() {
    set_id("juice") ;
    set_adj("strawberry") ;
    set_name("strawberry juice") ;
    set_short("a strawberry juice") ;
    set_long("A delicious strawberry juice.") ;
    set_mass(5) ;
    set_value(2) ;
    set_uses(5) ;
}
