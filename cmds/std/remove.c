/**
 * @file /cmds/std/remove.c
 * @description Command to remove worn items
 *
 * @created 2024/07/26 - Gesslar
 * @last_modified 2024/07/26 - Gesslar
 *
 * @history
 * 2024/07/26 - Gesslar - Created
 */

inherit STD_ACT ;

void setup() {
    help_text =
"Use this command to remove an item of clothing or armor that you are wearing. "
"For example, to remove a pair of boots, you would type 'remove boots'. You "
"may only remove items that you are wearing." ;
    usage_text = "remove <item>" ;
}

mixed main(object tp, string str) {
    object ob ;
    string slot ;
    string *slots ;
    string *items ;
    int i ;

    if(!ob = tp->query_equipment(str))
        return "You are not wearing that item." ;

    if(!ob->unequip())
        return "You cannot remove that item." ;

    return "You remove the "+ob_short(ob)+"." ;
}
