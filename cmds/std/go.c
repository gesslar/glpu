// /cmds/std/go.c
// Command for moving around in the game.
//
// Created:     2024/02/03: Gesslar
// Last Change: 2024/02/03: Gesslar
//
// 2024/02/03: Gesslar - Created

inherit STD_CMD ;

mixed main(object tp, object room, string arg) {
    mixed dest ;
    mixed result ;

    result = MOVE_D->allow_walk_direction(tp, room, arg) ;
    if(result != 1) {
        if(stringp(result))
            return result ;
        else
            return "You may not move in that direction." ;
    }

    result = MOVE_D->prevent_walk_direction(tp, room, arg) ;
    if(result != 0) {
        if(stringp(result))
            return result ;
        else
            return "You may not move in that direction." ;
    }

    if(!tp->move_living(dest, arg))
    return "You are not allowed to go there." ;

    MOVE_D->pre_walk_direction(tp, room, arg) ;
    MOVE_D->post_walk_direction(tp, dest, arg) ;

    return 1 ;
}
