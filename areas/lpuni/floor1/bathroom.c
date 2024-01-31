/* bathroom.c

 Tacitus @ LPUniversity
 10-NOV-05
 LPUniversity Cafe

*/

inherit STD_ROOM;

int lockFlag = 0;

int lock_door(string args);
int unlock_door(string args);
void auto_unlock();

void create()
{
    set_light(1);
    set_short("A small bathroom");
    set_long(
      "The bathroom has blue and white tiles, the walls are a soft\n"
      "blue. A toilet and a sink are positioned on either sides of\n"
      "the room. The door is strong and sturdy and their is a lock\n"
      "attached to it to allow you to lock or unlock the door.\n");
    set_items(([
      "sink" : "To wash up with\n",
      "toilet" : "To use to...\n",
      "door" : "@@query_door",
    ]) );

    set_exits(([
      "east" : __DIR__ + "backr.c",
    ]) );
}

void init()
{
     add_action("lock_door", "lock");
     add_action("unlock_door", "unlock");
}

string query_door()
{
    if(lockFlag == 0) return "The big oak door is unlocked.\n";
    else return "The big oak door is currently locked.\n";
}

int lock_door(string args)
{
     object caller = this_player() ;

     if(args != "door") return 0;

     lockFlag = 1;
     caller->tell("You lock the door.\n");
     tell(capitalize(this_player()->query_name()) + " locks the bathroom door.\n", caller);
     catch(find_object(query_exit("west"))->tell("You hear a click coming from the bathroom door.\n"));
     call_out("auto_unlock", 120);
     return 1;
}

int unlock_door(string args)
{
     object caller = this_player() ;

     if(args != "door") return 0;
     lockFlag = 0;
     caller->tell("You unlock the door.\n");
     tell(capitalize(this_player()->query_name()) + " unlocks the bathroom door.\n");
     catch(find_object(query_exit("west"))->tell("You hear a click coming from the bathroom door.\n"));
     return 1;
}

void auto_unlock()
{
     lockFlag = 0;
     tell("The bathroom door automatically unlocks.");
     catch(find_object(query_exit("west"))->tell("You hear a click coming from the bathroom door.\n"));
}

int receive_object(object ob)
{
     if(lockFlag == 0) return 1;
     else return 0;
}

int release_object(object ob)
{
     if(lockFlag == 0) return 1;
     else
     {
          write("Error [move]: The door is currently locked.\n");
          return 0;
     }
}
