/*

  bk.c
  Parthenon@LPUniversity
  August 19th, 2006
  Command to backup a file

*/

inherit STD_CMD ;

int main(object caller, object room, string arg)
{
    if(!arg || arg == "")
    return notify_fail("Error [back]: Syntax: back <file name>.\n");

    arg = resolve_path(caller->query("cwd"), arg);

    if(!file_exists(arg))
    return notify_fail("Error [back]: That file does not exist.\n");

    if(!(int)master()->valid_write(arg, caller, "cp"))
    return notify_fail("Error [back]: Permission denied.\n");

    write(cp(arg, arg + ".bak") ? "Success [back]: File successfully backed up.\n" :
        "Error [back]: File could not be backed up.\n");

    return 1;
}

string help(object caller)
{
    return
    " SYNTAX: back <filename>\n\n" +
    "This command allows you to create a backup copy of a file.\n" +
    "It will create a copy with the <filename> of '<filename>.back.\n";
}
