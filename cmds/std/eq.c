/**
 * @file /cmds/std/eq.c
 * @description List all your equipped items
 *
 * @created 2024/07/27 - Gesslar
 * @last_modified 2024/07/27 - Gesslar
 *
 * @history
 * 2024/07/27 - Gesslar - Created
 */

inherit STD_ACT ;

mixed main(object tp, string str) {
    mapping eq ;
    string slot, *slots ;
    object item ;
    string out = "" ;
    int sz, max ;

    eq = tp->query_equipped() ;

    if(!sizeof(eq))
        return "You are not equipping anything." ;

    out += "You have equipped the following items:\n\n" ;

    slots = keys(eq) ;
    max = max(map(slots, (: strlen :))) ;
    foreach(slot, item in eq)
        out += sprintf("%*s : %s\n", max, capitalize(slot), get_short(item)) ;

    return out ;
}
