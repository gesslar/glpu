/**
 * @file /cmds/std/open.c
 * @description Command to open a container or door
 *
 * @created 2024-08-07 - Gesslar
 * @last_modified 2024-08-07 - Gesslar
 *
 * @history
 * 2024-08-07 - Gesslar - Created
 */

inherit STD_ACT ;

mixed main(object tp, string args) {
  object room ;
  string target;
  object source ;
  int here_flag ;
  object ob;

  if(!args)
    return "Open what?";

  room = environment(tp) ;
  if(room->valid_door(args)) {
    string short, other_room_file ;
    object other_room ;

    if(room->query_door_locked(args))
      return "It's locked.";
    if(!room->query_door_open(args))
      room->set_door_open(args, true) ;
    else
      return sprintf("The way leading %s is already open.", args) ;

    short = room->query_door_short(args) ;
    tp->simple_action("$N $vopen the $o.\n", short) ;

    other_room = room->query_exit_dest(args) ;
    if(other_room) {
      foreach(string dir in other_room->query_exit_ids()) {
        if(other_room->query_exit_dest(dir) == room) {
            short = other_room->query_door_short(dir) ;
            other_room->set_door_open(dir, true) ;
            tell_down(other_room, sprintf("The %s opens.\n", short)) ;
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
    if(!ob = find_target(target, environment(tp)))
      return "You do not see that here.";
  } else {
    if(!ob = find_target(tp, target, tp))
      if(!ob = find_target(tp, target))
        return "You do not see " + target + " anywhere.";
  }

  if(!ob->closeable())
    return "You cannot open that.";

  if(!ob->is_closed())
    return "It is already open.";

  ob->set_closed(0);
  tp->simple_action("$N $vopen $o.\n", ob);

  return 1;
}
