/*

  functions.c
  Parthenon@LPUniversity
  August 16th, 2006
  command to list functions contained in an object

*/

// Last Change: 2024/02/04: Gesslar
// - general formatting

mixed main(object caller, string arg) {
    object ob, pager;
    string *funcs, ret;

    if(!arg || arg == "")
        return "Error: [functions]: SYNTAX: functions <object id>|<filename>.\n" ;
    ob = present(arg);

    if(!ob)
        ob = present(arg, environment(caller));

    if(!ob)
        ob = find_object(arg);

    if(!ob)
        return "Error: [functions]: Could not find object " + arg + ".\n" ;

    funcs = functions(ob);
    funcs = sort_array(funcs, 1);

    ret = sprintf("%-#79.3s\n\n", implode(funcs, "\n"));

    pager = new(OBJ_PAGER);

    pager->page(ret);
    return 1;
}

string help(object caller) {
    return
"SYNTAX: functions <object>\n\n" +
"This command will show the functions contained in <object> You may use the "
"id of the object if it is in your inventory or environment, otherwise you may "
"use the filename of the object to try and locate it.";
}
