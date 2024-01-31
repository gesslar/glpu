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
    help =
"work - Change your current working diretory to that of your environment\n"
"work <object> - Change your current working directory to that of the object\n"
;
}

mixed main(object tp, object room, string arg) {
    object ob ;
    string path ;

    if(!arg)
        ob = room ;
    else
        ob = get_object(arg) ;

    if(!ob)
        return "No such object: " + arg ;

    path = ob->query_directory() ;

    if(!directory_exists(path))
        return "No such directory: " + path ;

    tp->set_cwd(path) ;

    return "Success [work]: CWD set to " + path ;
}
