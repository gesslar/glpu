/* give.c

 Ico2 @ LPUniversity
 28-OCTOBER-05
 Standard command

QC Review: Tacitus on 29-OCT-05

Last edited on May 5th, 2006 by Tacitus

*/

inherit STD_CMD ;

void setup() {
     help_text =
"This command will allow you to give an item that you are currently holding "
"to another living in your current environment.";
     usage_text = "give <item> to <target>";
}

mixed main(object tp, string arg) {
     string targetname;
     object room ;
     string obname;
     object ob;
     object target;
     string *parts ;

     if(!arg)
          return "Give what to whom?" ;

     if(!sscanf(arg, "%s to %s", obname, targetname))
          return "Give what to whom?" ;

     room = environment(tp);
     if(!room)
          return "You are nowhere." ;

     target = find_local_target(tp, targetname);
     if(!target)
          return "You don't see " + targetname + " here.";

     if(!living(target))
          return "You can't give anything to that.";

     ob = find_carried_object(tp, obname);
     if(!objectp(ob))
          return "You don't see "+add_article(obname)+" here.";

     if(ob->prevent_transfer(target, room))
          return "You can't give that to "+target->query_name()+".";

     if(!(ob->move(target)) & MOVE_OK)
          return "You can't give that to "+target->query_name()+".";

     {
          string poss = possessive(tp);
          string short = get_short(ob);
          string name = tp->query_name();
          string vname = target->query_name();

          tell(tp, "You give your "+short+" to "+vname+".\n");
          tell(target, name+" gives you "+poss+" "+short+".\n");
          tell_down(room, name+" gives "+vname+" "+poss+" "+short+".\n");
     }

     return 1;
}
