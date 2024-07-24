// /cmds/adm/work.c
// Change your current working directory to one based on an object or
// environment
//
// Created:     2024/01/31: Gesslar
// Last Change: 2024/01/31: Gesslar
//
// 2024/01/31: Gesslar - Created

inherit STD_CMD ;

void setup() {
    help_text =
"work - Change your current working directory to that of your environment\n"
"work <object> - Change your current working directory to that of the object."
;
}

mixed main(object tp, string arg) {
    object ob ;
    string path ;
    object room = environment(tp) ;

    if(!arg)
        ob = room ;
    else
        ob = get_object(arg) ;

    if(!ob)
        return _error("No such object: %s", arg) ;

    path = query_directory(ob) ;

    if(!directory_exists(path))
        return _error("No such directory: %s", path) ;

    tp->set_env("cwd", path) ;

    return _ok("Current working directory set to: %s", path) ;
}
