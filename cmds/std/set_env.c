//set_env.c

//Tacitus @ LPUniversity
//24-OCT-05
//Standard user

/* Last edited on October 4th, 2006 by Tacitus */

inherit STD_CMD ;

int main(object caller, object room, string str) {
     string var_name, var_value, *keys;
     mapping data;
     int i;

     if(!str) {
          data = caller->list_env();
          if(!mapp(data)) return notify_fail("set_env: No environment values currently set.\n");
          else {
               keys = keys(data);
               keys = sort_array(keys, 1);
               if(sizeof(keys) <= 0) return notify_fail("set_env: No environment values currently set.\n");
               write("      >> ENVIRONMENT VARIABLES <<\n\n");
               for(i = 0; i < sizeof(keys); i ++)
                    tell(caller, sprintf("%-20s : %-20s\n", keys[i], data[keys[i]]), NO_ANSI) ;
               return 1;
          }
     }

    sscanf(str, "%s %s", var_name, var_value);
    if(!var_name) var_name = str;
    caller->set_env(var_name, var_value);
    if(!var_value) write("Success [set_var]: Value '" + var_name + "' cleared.\n");
    else write("Success [set_var]: Value '" + var_name + "' set to '" + var_value + "'.\n");
    return 1;
}

string help(object caller)
{
     return(
" SYNTAX: set_env [<var_name> <var_value>]\n\n"
"This command allows you, the user, to set environment values that effect the "
"way the mud interacts with you. If you do not provide an argument, then this "
"command will return information regarding your currently set environment "
"variables.\n\n"
"To set a variable, you would type 'set_env var_name var_value'. If you omit "
"the second argument and a environmental variable with a name matching the "
"first argument, then that environmental variable will be deleted.\n\n"
" See also: environmental variables, colour\n");
}
