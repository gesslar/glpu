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
    set_long(
        "This is the shop in Olum village. You can buy and sell items here."
    );

    init_shop() ;

    add_shop_inventory(
        "/obj/toycar"
    ) ;
}
