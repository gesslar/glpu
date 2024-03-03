// /obj/clothing/cream-coloured_linen_tunic.c
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_CLOTHING ;

void setup() {
    set_id(({"cream-coloured linen tunic", "tunic", "linen tunic"})) ;
    set_short("cream-coloured linen tunic") ;
    set_slot("torso") ;
    set_long("This tunic is made from a high-quality, cream-coloured linen "
        "that hangs loosely over the torso, falling to mid-thigh. Its sleeves "
        "are long and slightly billowy, cinching at the wrists. The neckline "
        "is modest, with a simple slit down the front that is fastened with "
        "small, wooden toggles. The fabric has a natural, subtle texture, "
        "hinting at its fine weave.") ;
    set_mass(50) ;
    set_bulk(20) ;
}
