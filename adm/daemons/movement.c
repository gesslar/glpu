// /adm/daemons/movement.c
// Daemon that provides movement checks, etc.
//
// Created:     2024/01/30: Gesslar
// Last Change: 2024/01/30: Gesslar
//
// 2024/01/30: Gesslar - Created

inherit STD_DAEMON ;

varargs mixed allow_walk_direction(object ob, object room, string dir) {
    if(!ob || !room || !dir)
        return 0 ;

    if(!room->valid_exit(dir))
        return "There is no exit in that direction." ;

    return 1 ;
}

varargs mixed prevent_walk_direction(object ob, object room, mixed dest) {
    if(!ob || !room || !dest)
        return 1 ;

    if(!room->can_release(ob))
        return "You cannot leave this room." ;

    if(!dest->can_receive(ob))
        return "You cannot enter that room." ;

    return 0 ;
}

varargs void pre_walk_direction(object who, object room, string dir) {
    if(!who || !room || !dir)
        return ;

    if(room->has_pre_exit_function(dir))
        room->evaluate_pre_exit_function(dir, who);
}

varargs void post_walk_direction(object who, object room, string dir) {
    if(!who || !room || !dir)
        return ;

    if(room->has_post_exit_function(dir))
        room->evaluate_post_exit_function(dir, who) ;
}
