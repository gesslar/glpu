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

    if(!room->valid_exit(arg))
        return "There is no exit " + arg + " here." ;

    dest = room->query_exit(arg) ;
    if(!tp->allow_move(dest))
        return "You are not allowed to go there." ;

    if(!tp->move_living(dest, arg))
        return "You are not allowed to go there." ;

    return 1 ;
}
