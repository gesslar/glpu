/**
 * @file /cmds/std/close.c
 * @description Command to close a container or door
 *
 * @created 2024-08-08 - Gesslar
 * @last_modified 2024-08-08 - Gesslar
 *
 * @history
 * 2024-08-08 - Gesslar - Created
 */

inherit STD_ACT ;

mixed main(object tp, string args) {
  string target;
  object source ;
  object room ;
  int here_flag ;
  object ob;

  if(!args)
    return "Close what?";

  room = environment(tp) ;
  if(room->valid_door(args)) {
    string short, other_room_file ;
    object other_room ;

    if(room->query_door_locked(args))
      return "It's locked.";
    if(room->query_door_open(args))
      room->set_door_open(args, false) ;
    else
      return sprintf("The way leading %s is already closed.", args) ;

    short = room->query_door_short(args) ;
    tp->simple_action("$N $vclose the $o.\n", short) ;

    other_room = room->query_exit_dest(args) ;
    if(other_room) {
      foreach(string dir in other_room->query_exit_ids()) {
        if(other_room->query_exit_dest(dir) == room) {
            short = other_room->query_door_short(dir) ;
            other_room->set_door_open(dir, false) ;
            tell_down(other_room, sprintf("The %s closes.\n", short)) ;
            break ;
        }
      }
    }
    return 1 ;
  }

  if(sscanf(args, "%s here", target) == 1)
    here_flag = 1;
  else
    target = args;

  if(here_flag) {
    if(!ob = find_target(target, room))
      return "You do not see that here.";
  } else {
    if(!ob = find_target(tp, target, tp))
      if(!ob = find_target(tp, target))
        return "You do not see " + target + " anywhere.";
  }

  if(!ob->closeable())
    return "You cannot close that.";

  if(ob->is_closed())
    return "It is already closed.";

  ob->set_closed(1);
  tp->simple_action("$N $vclose $o.\n", ob);

  return 1;
}
