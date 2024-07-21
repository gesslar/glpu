//cp.c

//Tacitus @ LPUniversity
//08-APR-05
//File system management

//Last edited on January 25th, 2006 by Tacitus

mixed main(object tp, string str) {
     string source, dest;

     if(!str || !sscanf(str, "%s %s", source, dest))
          return _error(tp, "Syntax: cp <source> <dest>");

     source = resolve_path(tp->query_env("cwd"), source);
     dest = resolve_path(tp->query_env("cwd"), dest);

     if(!file_exists(source))
          return _error("No such source file: %s", source);

     if(directory_exists(dest) || dest[<1..<1] == "/") {
          if(dest[<1..<1] != "/") dest += "/";

          if(strsrch(source, "/", -1) != -1) {
             dest += source[(strsrch(source, "/", -1) + 1)..<1];
          }

          else dest += source;
     }

     if(source == dest)
          return(_error("Destination may not match source."));

     if(!(int)master()->valid_write(dest, this_object(), "cp"))
          return _error("Permission Denied.");

     if(cp(source, dest) < 0)
          return _error(("Error [cp]: Copy failed."));
     else
          return _ok("%s copied to %s", source, dest);
}

string query_help(object tp) {
    return
"SYNTAX: cp <source> <destination>\n\n" +
"This command copies a file from it's current destination to a specified one.";
}
