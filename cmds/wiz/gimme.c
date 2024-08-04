/**
 * @file /cmds/wiz/gimme.c
 * @description Command to give coins to the wizard
 *
 * @created 2024/08/01 - Gesslar
 * @last_modified 2024/08/01 - Gesslar
 *
 * @history
 * 2024/08/01 - Gesslar - Created
 */

inherit STD_CMD ;

mixed main(object tp, string str) {
    string type ;
    int number ;

    if(!str)
        return "Usage: gimme <number> <type>\n" ;

    if(sscanf(str, "%d %s", number, type) != 2)
        return "Usage: gimme <number> <type>\n" ;

    if(number < 1)
        return "You must give at least one coin.\n" ;

    if(!of(type, CURRENCY_D->currency_list()))
        return "Invalid currency type.\n" ;

    if(!tp->add_wealth(type, number))
        return "You can't add that many coins.\n" ;

    return sprintf("You receive %d %s.\n", number, type) ;
}
