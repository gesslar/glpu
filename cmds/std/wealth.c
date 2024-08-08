// /cmds/std/wealth.c
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_CMD ;

mixed main(object tp, string arg) {
    string *currencies = CURRENCY_D->currency_list() ;
    string *out = ({ }) ;

    if(!sizeof(currencies))
        return "No currency is currently in use." ;

    currencies = reverse_array(currencies) ;
    foreach(string currency in currencies)
        out += ({ sprintf("%s: %d", currency, tp->query_wealth(currency)) }) ;

    out = ({ "You are carrying the following currencies:" }) + out ;

    return out ;
}
