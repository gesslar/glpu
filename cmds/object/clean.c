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

mixed main(object tp, string arg) {
     int i ;
     object target ;
     object ob, next ;

     if(!arg) target = tp ;
     else {
          if(arg[0]!='/') arg = resolve_path(tp->query_env("cwd"), arg) ;
          if(arg[<2..<1] != ".c") arg += ".c" ;
          if(!target) target = find_object(arg) ;
          if(!target) target = present(arg, tp) ;
          if(!target) target = present(arg, environment(tp)) ;
     }

     if(!target)
          return _error("Error locating target.") ;

     tp->_info("Destroying all objects in '" + get_short(target) + "'.\n") ;
     ob = first_inventory(target) ;
     while(ob) {
          string short = get_short(ob) ;

          next = next_inventory(ob) ;
          if(ob->query_no_clean() || ob->can_clean_up()) {
               ob = next ;
               continue ;
          }
          if(living(ob)) {
               ob = next ;
               continue ;
          }

          ob->remove() ;
          if(ob)
               destruct(ob) ;

          _ok(tp, "%s destroyed.", short) ;
          ob = next ;
     }

     return 1 ;
}

string help(object tp) {
     return
"SYNTAX: clean <object>\n\n"
"This command will destory all objects within another object (aka the objects "
"inventory). Your argument may be an object that is in your inventory, your "
"environment, or a filename. If you do not speicfy an object, it will remove "
"your inventory." ;
}
