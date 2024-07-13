/* clean.c

Icoz@LPUniversity (ico2ico2@gmail.com)
28-OCT-05
Object minipulation command

QC Review: Tacitus on October 28th, 2005

Last edited on October 29th, 2005 by Tacitus

*/

// Last Change: 2024/02/04: Gesslar
// - General formatting + refactor to use newer codes support.

inherit STD_CMD ;

mixed main(object caller, string arg) {
     int i;
     object target;
     object *inventory;

     if(!arg) target = caller;
     else {
          if(arg[0]!='/') arg = resolve_path(caller->query("cwd"), arg);
          if(arg[<2..<1] != ".c") arg += ".c";
          if(!target) target = find_object(arg);
          if(!target) target = present(arg, caller);
          if(!target) target = present(arg, environment(caller));
     }

     if(!target)
          return "Error [clean]: Error locating target." ;

     tell(caller, "Success [clean]: Destroying all objects in '" + get_short(target) + "'.\n");
     inventory = all_inventory(target);

     for(i=0; i<sizeof(inventory); i++) {
          if(inventory[i]->query("no_clean") || inventory[i]->can_clean_up()) continue;
          if(living(inventory[i])) continue;

          tell(caller, "* Object '" + get_short(inventory[i]) + "' destroyed.\n");
          inventory[i]->remove();
          if(inventory[i]) inventory[i]->remove() ;
     }

     return 1;
}

string help(object caller) {
     return
"SYNTAX: clean <object>\n\n"
"This command will destory all objects within another object (aka the objects "
"inventory). Your argument may be an object that is in your inventory, your "
"environment, or a filename. If you do not speicfy an object, it will remove "
"your inventory.";
}
