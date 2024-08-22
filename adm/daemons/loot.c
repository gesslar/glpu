/**
 * @file /adm/daemons/loot.c
 * @description This daemon is responsible for handling the loot system.
 *
 * @created 2024-08-05 - Gesslar
 * @last_modified 2024-08-05 - Gesslar
 *
 * @history
 * 2024-08-05 - Gesslar - Created
 */

#include <type.h>

inherit STD_DAEMON ;
inherit M_LOG ;

void drop_items(object tp, mixed item, object ob) ;
mixed process_loot_item(mixed item, object tp) ;
void drop_coins(object tp, mixed item, object ob) ;

void setup() {
    set_log_level(1) ;
}

void loot_drop(object tp, object ob) {
    mixed *loot_table, *loot;
    float chance ;

    loot_table = ob->query_loot_table();
    _debug("DEBUG: Loot table size: %d", sizeof(loot_table));

    if(sizeof(loot_table)) {
        foreach(loot in loot_table) {
            mixed item = loot[0] ;
            float roll ;
            chance = loot[1] ;

            _debug("DEBUG: Processing loot item: %O, chance: %f", item, chance);
            roll = random_float(100.0);
            _debug("DEBUG: Roll: %f", roll);

            if(roll < chance) {
                _debug("DEBUG: Roll successful, dropping item");
                catch(drop_items(tp, item, ob))  ;
            } else {
                _debug("DEBUG: Roll failed, item not dropped");
            }
        }
    }
}

void coin_drop(object tp, object ob) {
    mixed *coin_table, *loot;
    float chance ;
    float roll ;

    coin_table = ob->query_coin_table();
    _debug("DEBUG: Coin table size: %d", sizeof(coin_table));

    if(sizeof(coin_table)) {
        foreach(loot in coin_table) {
            mixed item = loot[0..1] ;
            chance = loot[2] ;

            _debug("DEBUG: Processing coin: %O, chance: %f", item, chance);
            roll = random_float(100.0);
            _debug("DEBUG: Roll: %f", roll);

            if(roll < chance)
                catch(drop_coins(tp, item, ob)) ;
            else
                _debug("DEBUG: Roll failed, coins not dropped");
        }
    }
}

void drop_items(object tp, mixed item, object ob) {
    mixed processed_item;
    object loot_ob;
    string file;
    mixed args;

    _debug("DEBUG: Dropping item: %O", item);
    processed_item = process_loot_item(item, tp);
    _debug("DEBUG: Processed item: %O", processed_item);

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
        int move_result;
        _debug("DEBUG: Attempting to move loot object: %O", loot_ob);
        move_result = loot_ob->move(ob);
        if(move_result) {
            _debug("DEBUG: Move failed, removing loot object");
            loot_ob->remove();
        } else {
            _debug("DEBUG: Move successful");
        }
    }
}

mixed process_loot_item(mixed item, object tp) {
    int size, index;
    mixed selected;

    _debug("DEBUG: Processing loot item: %O", item);

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

    _debug("DEBUG: Processed loot item result: %O", selected);
    return selected;
}

void drop_coins(object tp, mixed item, object ob) {
    string type;
    int num;
    object coin_ob ;
    int move_result;

    _debug("DEBUG: Dropping coins: %O", item);

    if(sizeof(item) == 2) {
        type = item[0];
        num = item[1];
        _debug("DEBUG: Coin type: %s, amount: %d", type, num);
    } else {
        _debug("DEBUG: Invalid coin item format");
        return;
    }

    coin_ob = new(OBJ_COIN, type, num);
    _debug("DEBUG: Created coin object: %O", coin_ob);

    move_result = coin_ob->move(ob);
    if(move_result) {
        _debug("DEBUG: Coin move failed, removing coin object");
        coin_ob->remove();
    } else {
        _debug("DEBUG: Coin move successful");
    }
}
