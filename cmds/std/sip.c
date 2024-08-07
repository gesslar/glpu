/**
 * @file /cmds/std/sip.c
 * @description Sip command
 *
 * @created 2024/08/06 - Gesslar
 * @last_modified 2024/08/06 - Gesslar
 *
 * @history
 * 2024/08/06 - Gesslar - Created
 */

inherit STD_ACT ;

mixed main(object tp, string str) {
    object ob ;
    int uses ;

    if(!ob = find_carried_object(tp, str))
        return "You don't have that." ;

    if(!ob->is_drink())
        return "You can't sip from that." ;

    uses = ob->query_uses() ;
    if(uses < 1)
        return "There is nothing left to sip." ;

    if(!ob->sip(tp))
        return "You couldn't sip from that." ;

    return 1 ;
}
