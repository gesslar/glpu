/* more.c

 Tacitus @ LPUniversity
 21-DEC-05
 File pager

Last edited 5-July-06 by Vicore

*/

inherit STD_CMD ;

void setup() {
  usage_text = "more <file>" ;
  help_text =
"This command will allow you to page files to your terminal, a screen at a "
"time. For more information about using this application and it's feature, "
"type 'help' within the pager.\n\n"
"Note: Colour is not parsed for any file ending with the\n"
"'.c' extension. All other files, however, will render with\n"
"colour.\n" ;
}

mixed main(object tp, string file) {
  string text ;

  if(!file && tp->query_env("cwf"))
    file = tp->query_env("cwf") ;
  else if(!file)
    return _usage(tp) ;

  file = resolve_file(tp, file) ;

  if(!file_exists(file))
    return _error("File '%s' does not exist.", file) ;

  text = read_file(file) ;

  text = "=== " + file + " ===\n" + text ;
  tp->page(text, null, 1) ;

  return 1 ;
}
