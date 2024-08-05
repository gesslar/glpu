// /obj/clothing/tan_leather_breeches.c
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_CLOTHING ;

void setup() {
    set_id(({"tan leather breeches", "breeches", "leather breeches"})) ;
    set_short("tan leather breeches") ;
    set_slot("legs") ;
    set_long("Crafted from supple, tan leather, these breeches fit snugly "
        "around the legs, allowing for ease of movement. The leather has been "
        "treated to a soft, almost buttery finish, and the seams are "
        "reinforced with precise, durable stitching. The breeches feature a "
        "high waist secured by a simple leather belt, and they taper neatly "
        "to the ankle.") ;
    set_mass(100) ;
}
