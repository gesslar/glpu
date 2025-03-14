/* inventory.c

 Tacitus @ LPUniversity
 28-OCT-05
 Standard command

*/

inherit STD_CMD;

string query_wealth(object tp);

mixed main(object tp, string args) {
    object *inventory;
    string *shorts;
    string wealth;
    int i;
    int fill, cap;
    object *equipped = values(tp->query_equipped());
    object *wielded = values(tp->query_wielded());

    inventory = find_targets(tp, args, tp);

    shorts = map(inventory, function(object ob, object tp, object *equipped, object *wielded) {
        string result;

        if(!ob->query_short())
            return 0;

        result = get_short(ob);

        if(of(ob, equipped))
            result += " (equipped)";
        else if(of(ob, wielded))
            result += " (wielded)";

        if(devp(tp))
            result += " (" + file_name(ob) + ")";

        return result;
    }, tp, equipped, wielded);

    shorts -= ({ 0 });

    if(sizeof(shorts) > 1)
        shorts += ({ "" });

    if(sizeof(shorts) > 0)
        shorts = ({ "Inventory:" }) + shorts;

    fill = tp->query_fill();
    cap = tp->query_capacity();

    wealth = query_wealth(tp);
    if(sizeof(wealth))
        shorts += ({ "Your purse contains " + wealth + ".", "" });

    shorts += ({ sprintf("Carrying: %d/%d", fill, cap) });

    return shorts;
}

string query_wealth(object tp) {
    string *currencies = CURRENCY_D->currency_list();
    string *out = ({ });

    if(!sizeof(currencies))
        return "No currency is currently in use.";

    currencies = reverse_array(currencies);
    foreach(string currency in currencies) {
        int num = tp->query_wealth(currency);
        if(num > 0)
            out += ({ sprintf("%d %s", tp->query_wealth(currency), currency) });
    }

    return implode(out, ", ");
}

string help(object tp) {
    return(" SYNTAX: inventory\n\n"
        "This command displays a list of everything that is currently\n"
        "in your inventory.\n");
}
