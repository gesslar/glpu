/* bathroom.c

 Tacitus @ LPUniversity
 10-NOV-05
 LPUniversity Cafe

*/

inherit STD_ROOM;

int lock_flag = 0;

int lock_door(string args);
int unlock_door(string args);
void auto_unlock();

void create() {
     set_light(1);
     set_short("A small bathroom");
     set_long(
          "The bathroom has blue and white tiles, the walls are a soft "
          "blue. A toilet and a sink are positioned on either sides of "
          "the room. The door is strong and sturdy and their is a lock "
          "attached to it to allow you to lock or unlock the door.");
     set_items(([
          "sink" : "To wash up with.",
          "toilet" : "To use to...",
          "door" : "@@query_door",
     ]) );

     set_exits(([
          "east" : __DIR__ + "backr",
     ]) );

     add_command("lock", "lock_door");
     add_command("unlock", "unlock_door");
}

// void init() {
//      add_action("lock_door", "lock");
//      add_action("unlock_door", "unlock");
// }

string query_door(string id) {
    if(lock_flag == 0) return "The big oak door is unlocked.\n";
    else return "The big oak door is currently locked.\n";
}

int lock_door(object tp, string args) {
     if(args != "door") return 0;

     lock_flag = 1;
     tell(tp, "You lock the door.\n");
     tell_all(this_object(),
          tp->query_cap_name() + " locks the bathroom door.\n", 0, tp);
     catch(load_object(query_exit("east"))->tell("You hear a click coming from the bathroom door.\n"));
     call_out("auto_unlock", 120);
     return 1;
}

int unlock_door(object tp, string args) {
debugf("unlock_door(%O, %O)", tp, args);
     if(args != "door") return 0;
     lock_flag = 0;
     tell(tp, "You unlock the door.\n");
     tell_all(this_object(), tp->query_cap_name() + " unlocks the bathroom door.\n", 0, tp);
     catch(load_object(query_exit("east"))->tell("You hear a click coming from the bathroom door.\n"));
     return 1;
}

void auto_unlock() {
     lock_flag = 0;
     tell_all(this_object(), "The bathroom door automatically unlocks.");
     catch(load_object(query_exit("east"))->tell("You hear a click coming from the bathroom door.\n"));
}

int can_receive(object ob) {
     if(lock_flag == 0) return 1;
     else return 0;
}

int can_release(object ob) {
     if(lock_flag == 0) return 1;
     else {
          write("Error [move]: The door is currently locked.\n");
          return 0;
     }
}
