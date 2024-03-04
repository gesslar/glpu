//cd.c

//Byre @ LPUniversity
//05-APR-05
//File Navigation

//Last edited on October 2nd, 2005 by Tacitus

inherit STD_CMD ;

int main(object caller, object room, string str)
{
     string *exp;

     if(!stringp(str))
     {
          if(directory_exists(user_path(caller->query_proper_name())))
          {
               str = user_path(caller->query_proper_name());
          }
          else
          {
               return notify_fail("Syntax: cd <directory>\n");
          }
     }
     if(str[0]!='/') str=resolve_path(caller->query("cwd"), str);
     exp=explode(str,"/");
     if(sizeof(exp)<=1)
     {
          if(!directory_exists(str)) return notify_fail("Error [cd]: Directory doesn't exist.\n");
          if(!(int)master()->valid_read(str, caller, "cd")) return notify_fail("Error [cd]: Permission denied.\n");
          caller->set("cwd",str);
          write(str+"\n");
          return 1;
     }

     if(exp[<1]=="..")
     {
          str="/"+implode(exp[0..<3],"/");
          if(str[0]!='/') str+="/";
          if(!directory_exists(str)) return notify_fail("Error [cd]: Directory doesn't exist.\n");
          if(!(int)master()->valid_read(str, caller, "cd")) return notify_fail("Error [cd]: Permission denied.\n");
          caller->set("cwd",str);
          write(str+"\n");
          return 1;
     }

     if(!(int)master()->valid_read(str, caller, "cd")) return notify_fail("Error [cd]: Permission denied.\n");
     if(!directory_exists(str)) return notify_fail("Error [cd]: Directory doesn't exist.\n");

     if(str[<1..] != "/") str += "/";
     caller->set("cwd",str);
     write(str+"\n");
     return 1;
}

string help(object caller) {
    return (" SYNTAX: cd <directory>\n\n" +
    "This command allows you to navigate through various directories.\n" +
    "To use this command, you simply provide the directory (either\n" +
    "an absolute path or one relative to the current directory) as the\n" +
    "argument to this command.\n\n" +
    "See also:  mkdir, rmdir, ls\n");
}
