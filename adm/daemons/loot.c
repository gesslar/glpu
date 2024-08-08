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

void drop_items(object tp, mixed item, object ob) ;
mixed process_loot_item(mixed item, object tp) ;
void drop_coins(object tp, mixed item, object ob) ;

void loot_drop(object tp, object ob) {
    mixed *loot_table, *loot;
    float chance ;

    loot_table = ob->query_loot_table();

    if(sizeof(loot_table)) {
        foreach(loot in loot_table) {
            mixed item = loot[0] ;
            chance = loot[1] ;

            if(random_float(100.0) < chance) {
                catch(drop_items(tp, item, ob))  ;
            }
        }
    }
}

void coin_drop(object tp, object ob) {
    mixed *coin_table, *loot;
    float chance ;

    coin_table = ob->query_coin_table();
    if(sizeof(coin_table)) {
        foreach(loot in coin_table) {
            mixed item = loot[0..1] ;
            chance = loot[2] ;

            if(random_float(100.0) < chance)
                catch(drop_coins(tp, item, ob)) ;
        }
    }
}

void drop_items(object tp, mixed item, object ob) {
    mixed processed_item;
    object loot_ob;
    string file;
    mixed args;

    processed_item = process_loot_item(item, tp);

    if(pointerp(processed_item) && sizeof(processed_item) == 2) {
        file = processed_item[0];
        args = processed_item[1];
        if(pointerp(args))
            loot_ob = new(file, args...);
        else
            loot_ob = new(file, args);
    } else if(stringp(processed_item)) {
        file = processed_item;
        loot_ob = new(file);
    } else {
        return;
    }

    if(loot_ob) {
        // Move returns 0 on success, anything else is a failure
        if(loot_ob->move(ob))
            loot_ob->remove();
    }
}

mixed process_loot_item(mixed item, object tp) {
    int size, index;
    mixed selected;

    if(valid_function(item)) {
        return process_loot_item((*item)(tp), tp);
    }
    if(pointerp(item)) {
        size = sizeof(item);
        if(size == 0) return 0;

        index = random(size);
        selected = item[index];

        if(stringp(selected)) {
            if(index < size - 1 && pointerp(item[index + 1])) {
                return ({ selected, item[index + 1] });
            } else {
                return selected;
            }
        } else if(pointerp(selected) && index > 0 && stringp(item[index - 1])) {
            return ({ item[index - 1], selected });
        }

        return 0;
    }
    if(mapp(item)) {
        return process_loot_item(element_of_weighted(item), tp);
    }
    if(stringp(item)) {
        return item;
    }

    return 0;
}

void drop_coins(object tp, mixed item, object ob) {
    string type;
    int num;
    object coin_ob ;
    int result ;

    if(sizeof(item) == 2) {
        type = item[0];
        num = item[1];
    } else
        return;

    coin_ob = new(OBJ_COIN, type, num);
    if(coin_ob->move(ob))
        coin_ob->remove();
}
