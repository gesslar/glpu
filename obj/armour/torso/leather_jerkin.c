/**
 * @file /obj/armour/torso/leather_jerkin.c
 * @description Basic leather armour
 *
 * @created 2024-07-27 - Gesslar
 * @last_modified 2024-07-27 - Gesslar
 *
 * @history
 * 2024-07-27 - Gesslar - Created
 */

inherit STD_ARMOUR;

void setup() {
    set_id(({"sturdy leather jerkin","leather jerkin", "jerkin", "armour"}));
    set_short("sturdy leather jerkin");
    set_name("sturdy leather jerkin");
    set_long("This sturdy leather jerkin is crafted from high-quality, "
             "supple leather, designed to offer both protection and "
             "mobility. The jerkin is dyed a rich, earthy brown, and the "
             "leather is treated to provide resistance against wear and "
             "tear. Reinforced stitching runs along the seams, ensuring "
             "durability, while the inside is lined with a softer material "
             "for added comfort. The jerkin features adjustable straps "
             "along the sides and shoulders, allowing for a snug fit.");
    set_mass(200);
    set_slot("torso");
    set_ac(2.0);
    set_defense(([
        "slashing" : 2.0,
        "piercing" : 1.0,
        "bludgeoning" : 1.0,
    ]));

    set_value(25);
}
