/* inventory.c

 Tacitus @ LPUniversity
 28-OCT-05
 Standard command

*/

inherit STD_CMD ;

mixed main(object caller, object room, string args) {
    object *inventory;
    string *shorts ;
    int i;

    inventory = all_inventory(caller);
    if(sizeof(inventory) < 1)
        return "You are not currently carrying anything.\n";

    shorts = map(inventory, (: get_short :) );
    shorts -= ({ 0 });

    if(sizeof(shorts) < 1)
        return "You are not currently carrying anything.\n";

    shorts = map(shorts, (: "    " + $1 :) );
    shorts = ({ "You are carrying the following:" }) + shorts;
    return shorts ;
}

string help(object caller) {
    return(" SYNTAX: inventory\n\n"
        "This command displays a list of everything that is currently\n"
        "in your inventory.\n");
}
