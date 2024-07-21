//cd.c

//Byre @ LPUniversity
//05-APR-05
//File Navigation

//Last edited on October 2nd, 2005 by Tacitus

inherit STD_CMD ;

mixed main(object tp, string str) {
     string test, *exp;

     test = str ;

     if(!stringp(test)) {
          test = user_path(tp);
          if(!directory_exists(test = user_path(tp)))
              return _error("Syntax: cd <directory>");
     }

     test = resolve_path(tp->query_env("cwd"), test);
     if(!directory_exists(test))
          test = str ;

     if(!directory_exists(test))
          return _error("No such directory: %s", test);

     if(!master()->valid_read(test, tp, "cd"))
          return _error("Permission denied.");

     tp->set_env("cwd", test);

     return _ok("Current directory set to: %s", test);
}

string query_help(object tp) {
    return
"SYNTAX: cd [directory]\n\n" +
"This command allows you to navigate through various directories. To use this "
"command, you simply provide the directory, either an absolute path or one "
"relative to the current directory, as the argument to this command.\n\n"
"Typing 'cd' without any arguments will take you to your home directory.\n\n"
"See also: mkdir, rmdir, ls";
}
