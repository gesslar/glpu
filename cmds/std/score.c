/**
 * @file /cmds/std/score.c
 * @description Score command
 *
 * @created 2024/08/06 - Gesslar
 * @last_modified 2024/08/06 - Gesslar
 *
 * @history
 * 2024/08/06 - Gesslar - Created
 */

inherit STD_CMD;

mixed main(object tp, string str) {
    string result ;

    result = sprintf("You are %s, a level %d %s.\n",
        capitalize(tp->query_proper_name()),
        to_int(tp->query_level()),
        tp->query_race()) ;

    result += sprintf("You have %.2f hp, %.2f sp, and %.2f mp.\n",
        tp->query_hp(),
        tp->query_sp(),
        tp->query_mp()) ;

    result += sprintf("Your have %s experience points and require %s "
        "to advance.\n",
        add_commas(tp->query_xp()),
        add_commas(tp->query_tnl())) ;

    return result ;
}
