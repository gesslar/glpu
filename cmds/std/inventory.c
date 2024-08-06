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
    object *equipped = values(tp->query_equipped()) ;
    object *wielded = values(tp->query_wielded()) ;

    inventory = all_inventory(tp);
    inventory = filter(inventory, (: $2->can_see($1) :), tp) ;

    shorts = map(inventory, function(object ob, object *equipped, object *wielded) {
        if(!ob->query_short())
            return 0 ;

        if(of(ob, equipped))
            return sprintf("%s (worn)", get_short(ob)) ;
        else if(of(ob, wielded))
            return sprintf("%s (wielded)", get_short(ob)) ;
        else
            return get_short(ob) ;
    }, equipped, wielded);

    shorts -= ({ 0 }) ;

    if(sizeof(shorts) > 1)
        shorts += ({ "" }) ;

    if(sizeof(shorts) > 0)
        shorts = ({ "Inventory:" }) + shorts ;

    cap = tp->query_capacity() ;
    max_cap = tp->query_max_capacity() ;

    wealth = query_wealth(tp) ;
    if(sizeof(wealth))
        shorts += ({ "Your purse contains " + wealth + ".", "" }) ;

    shorts += ({ sprintf("Capacity: %d/%d", max_cap-cap, max_cap) }) ;

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
            out += ({ sprintf("%d %s", tp->query_wealth(currency), currency) }) ;
    }

    return implode(out, ", ") ;
}

string help(object tp) {
    return(" SYNTAX: inventory\n\n"
        "This command displays a list of everything that is currently\n"
        "in your inventory.\n");
}
