/* more.c

 Tacitus @ LPUniversity
 21-DEC-05
 File pager

Last edited 5-July-06 by Vicore

*/

inherit STD_CMD ;

object pager;

mixed main(object tp, string file) {
    string text;

    if(!file && this_interactive()->query_env("cwf")) file = this_interactive()->query_env("cwf");
    else if(!file) return(notify_fail("SYNTAX: more <file>\n"));
    file = resolve_path(tp->query_env("cwd"), file);
    if(!file_exists(file)) return(notify_fail("Error [more]: File '" + file + "' does not exist.\n"));
    text = read_file(file);

    tp->page(text, null, 1) ;

    return 1;
}

string help(object tp)
{
    return("SYNTAX: more <file>\n\n" +
      "This command will allow you to page files to your terminal,\n"
      "a screen at a time. For more information about using this\n"
      "application and it's feature, type 'help' within the pager.\n\n"
      "Note: Colour is not parsed for any file ending with the\n"
      "'.c' extension. All other files, however, will render with\n"
      "colour.\n");
}
