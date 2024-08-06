/**
 * @file /adm/daemons/loot.c
 * @description This daemon is responsible for handling the loot system.
 *
 * @created 2024/08/05 - Gesslar
 * @last_modified 2024/08/05 - Gesslar
 *
 * @history
 * 2024/08/05 - Gesslar - Created
 */

#include <type.h>

inherit STD_DAEMON ;

void drop(object tp, mixed item, object ob) ;
mixed process_loot_item(mixed item, object tp) ;

void loot_drop(object tp, object ob) {
    mixed *loot_table, *loot;
    float chance ;

    loot_table = ob->query_loot_table();

    if(!sizeof(loot_table))
        return;

    foreach(loot in loot_table) {
        mixed item = loot[0] ;
        chance = loot[<1] ;

        if(random_float(100.0) < chance) {
            catch(drop(tp, item, ob))  ;
        }
    }
}

void drop(object tp, mixed item, object ob) {
    mixed processed_item;
    object loot_ob;
    string file;
    mixed args;

    processed_item = process_loot_item(item, tp);

    if (pointerp(processed_item) && sizeof(processed_item) == 2) {
        file = processed_item[0];
        args = processed_item[1];
        if (pointerp(args))
            loot_ob = new(file, args...);
        else
            loot_ob = new(file, args);
    } else if (stringp(processed_item)) {
        file = processed_item;
        loot_ob = new(file);
    } else {
        return;
    }

    if (loot_ob) {
        // Move returns 0 on success, anything else is a failure
        if (loot_ob->move(ob))
            loot_ob->remove();
    }
}

mixed process_loot_item(mixed item, object tp) {
    int size, index;
    mixed selected;

    if (valid_function(item)) {
        return process_loot_item((*item)(tp), tp);
    }
    if (pointerp(item)) {
        size = sizeof(item);
        if (size == 0) return 0;

        index = random(size);
        selected = item[index];

        if (stringp(selected)) {
            if (index < size - 1 && pointerp(item[index + 1])) {
                return ({ selected, item[index + 1] });
            } else {
                return selected;
            }
        } else if (pointerp(selected) && index > 0 && stringp(item[index - 1])) {
            return ({ item[index - 1], selected });
        }

        return 0;
    }
    if (mapp(item)) {
        return process_loot_item(element_of_weighted(item), tp);
    }
    if (stringp(item)) {
        return item;
    }

    return 0;
}
#if 0
void runit() {
    object fake_player = this_player() ;
    object fake_container = load_object("/std/modules/loot") ;

    mixed *test_loot_tables = ({
        ({ ({ "/obj/sword", 100.0 }) }),
        ({ ({ ({ "/obj/sword", "/obj/shield" }), 100.0 }) }),
        ({ ({ ({ "/obj/potion", ({ "healing", 50 }) }), 100.0 }) }),
        ({ ({ ([ "/obj/gem" : 1, "/obj/coin" : 3 ]), 100.0 }) }),
        ({ ({ "/obj/armor", 100.0 }), ({ "/obj/weapon", 100.0 }) }),
        ({ ({ ({ "/obj/food", ({ "apple", 5 }) }), 100.0 }) }),
        ({ ({ "/obj/key", 100.0 }), ({ "/obj/map", 100.0 }) }),
        ({ ({ ([ "/obj/ring" : 1, "/obj/amulet" : 2, "/obj/bracelet" : 3 ]), 100.0 }) }),
        ({ ({ ({ "/obj/scroll", ({ "fireball", 3 }) }), 100.0 }), ({ "/obj/wand", 100.0 }) }),
        ({ ({ "/obj/trap", 100.0 }), ({ "/obj/chest", 100.0 }), ({ "/obj/gold", 100.0 }) })
    });

    printf("Starting loot drop test\n");
    printf("----------------------\n");

    foreach(mixed *table in test_loot_tables) {
        printf("\nTesting loot table: %O\n", table);
        fake_container->set_loot_table(table);
        loot_drop(fake_player, fake_container);
        printf("----------------------\n");
    }

    printf("Loot drop test complete\n");
}
#endif
