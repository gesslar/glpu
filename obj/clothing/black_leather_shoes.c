// /obj/clothing/black_leather_shoes.c
// Basic black leather shoes
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_CLOTHING ;

void setup() {
    set_id(({"shoes", "black shoes", "leather shoes", "black leather shoes"})) ;
    set_short("pair of black leather shoes") ;
    set_slot("feet") ;
    set_long("These shoes are made from premium, black leather that has been "
        "polished to a high shine. They are styled in a classic, understated "
        "design with a rounded toe and a low heel. The shoes fasten with "
        "thin, black laces that thread through small, silver eyelets, "
        "providing a secure and adjustable fit. The soles are thick and "
        "sturdy, suggesting durability and comfort for prolonged wear.") ;
    set_mass(15) ;
}
