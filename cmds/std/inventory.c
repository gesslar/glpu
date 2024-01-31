/* inventory.c

 Tacitus @ LPUniversity
 28-OCT-05
 Standard command

*/

inherit CMD ;

mixed main(object caller, object room, string args) {
    object *inventory;
    int i;

    inventory = all_inventory(caller);
    if(sizeof(inventory) < 1) return "You are not currently carrying anything.\n";
    write("You are carrying the following " + (sizeof(inventory) == 1 ? "item:\n" : sizeof(inventory) + " items:\n"));
    for(i = 0; i < sizeof(inventory); i ++)
    printf(" %s\n", get_short(inventory[i]));
    return 1;
}

string help(object caller)
{
    return(" SYNTAX: inventory\n\n"
      "This command displays a list of everything that is currently\n"
      "in your inventory.\n");
}
