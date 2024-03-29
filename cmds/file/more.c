/* more.c

 Tacitus @ LPUniversity
 21-DEC-05
 File pager

Last edited 5-July-06 by Vicore

*/

inherit STD_CMD ;

object pager;

int main(object caller, object room, string file)
{
    string text;
    pager = new("/obj/mudlib/pager/pager.c");
    if(!file && this_interactive()->query("cwf")) file = this_interactive()->query("cwf");
    else if(!file) return(notify_fail("SYNTAX: more <file>\n"));
    file = resolve_path(caller->query("cwd"), file);
    if(!file_exists(file)) return(notify_fail("Error [more]: File '" + file + "' does not exist.\n"));
    text = read_file(file);
    if(file[<2..<1] == ".c") pager->page(text, file, 0, 1);
    else if(file[<2..<1] == ".h") pager->page(text, file, 0, 1);
    else pager->page(text, file);
    return 1;
}

string help(object caller)
{
    return("SYNTAX: more <file>\n\n" +
      "This command will allow you to page files to your terminal,\n"
      "a screen at a time. For more information about using this\n"
      "application and it's feature, type 'help' within the pager.\n\n"
      "Note: Ansi is not parsed for any file ending with the\n"
      "'.c' extension. All other files, however, will render with\n"
      "colour.\n");
}
