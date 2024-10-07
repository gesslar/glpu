//cat.c

//Tacitus @ LPUniveristy
//02-OCT-05
//Pages entire file at once

inherit STD_CMD ;

void setup() {
    usage_text = "cat <file>" ;
    help_text =
"This command pages through an entire file at once." ;
}

mixed main(object tp, string str) {
  string *lines ;
  int i, max_lines ;

  if(!str)
    return _usage(tp) ;

  str = resolve_file(tp, str) ;

  if(!file_exists(str))
    return _error("No such file: %s", str) ;

  i = file_size(str) ;
  if(i < 0)
    return _error("No such file or 0-length file: %s", str) ;

  max_lines = file_length(str) ;

  tell(tp, "=== " + str + " ===\n") ;

  i = 0 ;

  while(++i <= max_lines)
    tell(tp, append(read_file(str, i, 1), "\n")) ;

  return 1 ;
}
