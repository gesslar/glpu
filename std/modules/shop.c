/**
 * @file /std/modules/shop.c
 * @description Module to be inherited by shops
 *
 * @created 2024/08/01 - Gesslar
 * @last_modified 2024/08/01 - Gesslar
 *
 * @history
 * 2024/08/01 - Gesslar - Created
 */

#include <classes.h>
#include "/std/object/include/object.h"
#include "/std/object/include/command.h"

inherit M_CURRENCY ;

inherit CLASS_STORAGE ;

void add_shop_inventory(mixed args) ;
mixed query_cost(object tp, object ob, string transaction) ;
protected void remove_shop() ;
protected void reset_shop() ;
private nomask object create_storage() ;

protected nosave int shop_open = 1;
protected nosave int allow_npcs = 0;
protected nosave float sell_factor = 0.5; // when a player sells, use this
                                          // factor to determine the price
protected nosave string shop_keep_file ;
protected nosave object store ;
private nosave mixed *shop_inventory = ({}) ;

void init_shop() {
    add_command("buy", "cmd_buy") ;
    add_command("sell", "cmd_sell") ;
    add_command("list", "cmd_list") ;

    create_storage() ;

    add_reset((:reset_shop:)) ;
    add_destruct((:remove_shop:)) ;
}

protected void remove_shop() {
    if(objectp(store))
        store->remove() ;
}

void add_shop_inventory(mixed args...) {
    mixed arg ;

    if(!pointerp(args))
        args = ({ args }) ;

    foreach(arg in args) {
        shop_inventory += ({ arg }) ;
    }
}

protected void reset_shop() {
    mixed arg ;

    create_storage() ;
    store->clean_contents() ;

    foreach(arg in shop_inventory) {
        string file ;
        int number ;
        mixed *clone_args ;
        int sz ;
        object ob ;

        if(!pointerp(arg))
            arg = ({ arg }) ;

        sz = sizeof(arg) ;

        file = arg[0] ;
        if(!stringp(file))
            continue ;

        if(sz > 1) {
            number = arg[1] ;
            if(sz > 2)
                clone_args = clone_args[2..] ;
        } else {
            number = 1 ;
        }

        while(number--) {
            string e ;

            e = catch {
                if(sizeof(clone_args))
                    ob = new(file, clone_args...) ;
                else
                    ob = new(file) ;
            } ;

            if(e) {
                log_file("shop_errors", e) ;
                continue ;
            }

            if(!objectp(ob))
                continue ;

            if(ob->move(store))
                ob->remove() ;
        }
    }
}

mixed cmd_list(object tp) {
    object *items, item ;
    string *lines = ({}) ;
    string line ;
    object ob ;
    string short ;
    mixed *cost ;

    create_storage() ;

    items = all_inventory(store) ;

    lines = ({ get_short(), "" }) ;

    foreach(item in items) {
        cost = query_cost(tp, item, "list") ;
        short = get_short(item) ;
        line = sprintf("%s (%d %s)", short, cost[0], cost[1]) ;
        lines += ({ line }) ;
    }

    return lines ;
}

mixed cmd_buy(object tp, string str) {
    object ob ;
    mixed result ;
    string action ;
    mixed cost ;
    string short ;
    mixed *paid, *change ;

    create_storage() ;

    if(!allow_npcs && !userp(tp))
        return 0 ;

    if (!shop_open)
        return "The shop is closed.";

    if(!userp(tp))
        return "Only players can buy from the shop.";

    if(!ob = present(str, store))
        return "The shop does not have that item.";

    cost = query_cost(tp, ob, "buy") ;

    result = handle_transaction(tp, cost[0], cost[1]) ;

    if(stringp(result))
        return result ;

    if(ob->move(tp)) {
        reverse_transaction(tp, result) ;

        return "You can't carry that much weight.";
    }

    short = get_short(ob) ;
    tp->other_action("$N $vbuy %o.", short) ;

    paid = result[0] ;
    change = result[1] ;

    action = "$N $vbuy $o for $o1" ;
    if(sizeof(change))
        action += " and receive $o2 in change" ;

    action += ".\n" ;

    tp->my_action(action,
        short,
        format_return_currency_string(paid),
        format_return_currency_string(change)
    ) ;

    return 1 ;
}

mixed cmd_sell(object tp, string str) {
    object ob ;
    mixed result ;
    string action ;
    mixed cost ;
    string short ;
    mixed *paid, *change ;
    int coin_mass ;

    create_storage() ;

    if(!allow_npcs && !userp(tp))
        return 0 ;

    if (!shop_open)
        return "The shop is closed.";

    if(!userp(tp))
        return "Only players can sell to the shop.";

    if(!ob = find_target(tp, str, tp))
        return "You don't have that item.";

    cost = query_cost(tp, ob, "sell") ;
    coin_mass = cost[0] ;

    if(ob->move(store))
        return "The shop refuses your item." ;

    if(nullp(tp->adjust_wealth(cost[1], cost[0]))) {
        ob->move(tp) ;
        return "You are overburdened and cannot carry the coins." ;
    }

    tp->other_action("$N $vsell %o.", get_short(ob)) ;
    tp->my_action("$N $vsell $o for $o1.\n",
        get_short(ob),
        format_currency(cost[0], cost[1])) ;

    return 1 ;
}

mixed *query_cost(object tp, object ob, int transaction) {
    mixed *value = ob->query_value() ;

    switch(transaction) {
        case "buy":
            return value ;
        case "sell":
            return ({ to_int(to_float(value[0]) * sell_factor), value[1] }) ;
        case "list":
            return value ;
    }
    return ob->query_value() ;
}

private nomask object create_storage() {
    if(objectp(store))
        return store ;

    return store = new(
        STD_STORAGE_OBJECT,
        new(class StorageOptions, clean_on_empty: 1)
    ) ;
}
