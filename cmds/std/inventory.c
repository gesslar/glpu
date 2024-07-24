/* inventory.c

 Tacitus @ LPUniversity
 28-OCT-05
 Standard command

*/

inherit STD_CMD ;

string query_wealth(object tp) ;

mixed main(object tp, string args) {
    object *inventory;
    string *shorts ;
    string wealth ;
    int i;
    int cap, max_cap ;
    int vol, max_vol ;

    inventory = all_inventory(tp);
    // if(sizeof(inventory) < 1)
    //     return "You are not currently carrying anything.\n";

    shorts = map(inventory, (: get_short :) );
    shorts -= ({ 0, "" });

    if(sizeof(shorts) > 1)
        shorts += ({ "" }) ;

    if(sizeof(shorts) > 0)
        shorts = ({ "Inventory:" }) + shorts + ({ "" }) ;

    cap = tp->query_capacity() ;
    max_cap = tp->query_max_capacity() ;

    vol = tp->query_volume() ;
    max_vol = tp->query_max_volume() ;

    wealth = query_wealth(tp) ;
    if(sizeof(wealth))
        shorts += ({ "Coin purse: " + wealth, "" }) ;

    shorts += ({ sprintf("Capacity: %d/%d", cap, max_cap) }) ;
    shorts += ({ sprintf("  Volume: %d/%d", vol, max_vol) }) ;

    return shorts ;
}

string query_wealth(object tp) {
    string *currencies = CURRENCY_D->currency_list() ;
    string *out = ({ }) ;

    if (!sizeof(currencies))
        return "No currency is currently in use." ;

    currencies = reverse_array(currencies) ;
    foreach(string currency in currencies) {
        int num = tp->query_wealth(currency) ;
        if(num > 0)
            out += ({ sprintf("%s: %d", currency, tp->query_wealth(currency)) }) ;
    }

    return implode(out, ", ") ;
}

string help(object tp) {
    return(" SYNTAX: inventory\n\n"
        "This command displays a list of everything that is currently\n"
        "in your inventory.\n");
}
