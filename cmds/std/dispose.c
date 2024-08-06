/**
 * @file /cmds/std/dispose.c
 * @description Get rid of dead bodies!
 *
 * @created 2024/08/06 - Gesslar
 * @last_modified 2024/08/06 - Gesslar
 *
 * @history
 * 2024/08/06 - Gesslar - Created
 */

inherit STD_ACT ;

mixed main(object tp, string str) {
    object ob, room ;

    if(!str)
        return "Dispose of what?" ;

    if(!ob = find_local_target(tp, str))
        return 1 ;

    room = environment(tp) ;
    if(!present(ob, room))
        return "You do not see that here." ;

    if(!ob->is_corpse())
        return "You cannot dispose of that." ;

    tp->simple_action("Quickly and quietly, $n $vwork to dispose of the $o.", ob) ;
    if(!ob->remove())
        return "You could not dispose of that." ;

    return 1 ;
}
