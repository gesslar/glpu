/**
 * @file /d/village/shop.c
 * @description A basic shop in Olum village.
 *
 * @created 2024/08/01 - Gesslar
 * @last_modified 2024/08/01 - Gesslar
 *
 * @history
 * 2024/08/01 - Gesslar - Created
 */

inherit __DIR__ "village_inherit" ;

inherit M_SHOP ;

void setup() {
    set_short("Olum Village Shop");
    set_long("The small, rustic shop, marked by a weathered wooden sign, "
             "emanates a cozy charm. Rough-hewn shelves made of dark oak are "
             "packed with a motley assortment of goods, from simple tools to "
             "peculiar trinkets. A sturdy wooden counter, worn smooth by "
             "years of use, sits near the entrance, where the friendly "
             "shopkeeper greets visitors with a warm smile. The scent of old "
             "wood and herbs fills the air, adding to the inviting, homely "
             "atmosphere. Here, amidst the humble surroundings, one can find "
             "everyday necessities and rare finds, each item with a story "
             "waiting to be discovered.");
    set_exits(([
        "west" : "village_path1",
    ])) ;

    init_shop() ;

    add_shop_inventory(
        "/obj/toycar",
        "/obj/weapon/piercing/rusty_sword",
        "/obj/armour/torso/leather_jerkin"
    ) ;
}
