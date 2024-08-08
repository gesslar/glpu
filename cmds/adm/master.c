/**
 * @file /cmds/adm/master.c
 * @description Command to reload core system files.
 *
 * @created 2024/07/17 - Gesslar
 * @last_modified 2024/07/17 - Gesslar
 *
 * @history
 * 2024/07/17 - Gesslar - Created
 */

inherit STD_CMD ;

private nosave nomask function *actions ;

void setup() {
     actions = ({
          (: destruct(master()) :),
          (: destruct(find_object("/adm/obj/master/valid")) :),
          (: load_object("/adm/obj/master/valid") :),
          (: load_object("/adm/obj/master") :),
          (: destruct(find_object(SIMUL_OB)) :),
          (: load_object(SIMUL_OB) :),
          (: CONFIG_D->rehash_config() :),
     }) ;
}

mixed main(object caller, string arguments) {
     function action  ;
     string err = "" ;

     if(!adminp(previous_object()))
          return _error("Access denied.\n");

     foreach(action in actions) {
          mixed result ;

          result = catch((*action)()) ;
          if(result)
               err += result ;
     }

     if(sizeof(err))
          return _error("Error when reloading system objects:\n\n"
               "%s\n"
               "Refer to \ebl1\e\eul1\e%scatch\eres\e",
               err,
               log_dir()
          ) ;
     else
          return _ok("System objects updated successfully.") ;
}

string query_help(object caller) {
     return (
" SYNTAX: master\n\n"
"This command reloads core system files. No argument is taken.\n");
}
