//cat.c

//Tacitus @ LPUniveristy
//02-OCT-05
//Pages entire file at once

inherit STD_CMD ;

mixed main(object caller, string str) {
     string *lines;
     int i, max_lines ;

     if(!str)
          return _error("No file specified.") ;

     str = resolve_path(caller->query_env("cwd"),str);

     if(!file_exists(str))
          return _error("No such file: %s", str);

     i = file_size(str);
     if(i < 0)
          return _error("No such file or 0-length file: %s", str);

     max_lines = file_length(str);

     i = 0;
     while(++i <= max_lines)
          tell(caller, append(read_file(str, i, 1), "\n"));

     return 1;
}

string help(object caller) {
     return (" SYNTAX: cat <file>\n\n" +
    "This command pages through an entire file at once.\n");

}
