//update.c

//Byre @ LPUniversity
//05-APR-05
//Object management

//Last edited July 11th, 2006 by Tacitus
// Last Change: 2024/02/04: Gesslar
// - Added recursion to the update command

inherit STD_CMD ;

void do_update(string file) ;

mixed main(object caller, object room, string arg) {
    object obj;
    string error, *users;
    string *files, file, *parts, start ;
    int depth = 0 ;

    if(arg) {
        if(arg == "-r" || arg == "-R") {
            if(arg == "-r") depth = 1 ;
            else if(arg == "R") depth = 2 ;
        } else if(sizeof(parts = pcre_extract(arg, "^-([rR]) (.*)$")) == 2) {
            if(parts[0] == "r") depth = 1 ;
            else depth = 2 ;

            file = parts[1] ;
        } else {
            depth = 0 ;
            file = arg ;
        }
    }

    if(!file) {
        if(!caller->query("cwf"))
            return "Error [update]: You must provide an argument. Syntax: update [-rR] [<file>]\n" ;
        file = caller->query("cwf");
    }

    file = resolve_path(caller->query("cwd"), file);
    file = append(file, ".c") ;
    if(file == append(file_name(), ".c")) {
        return "Error [update]: You cannot update the update command. Destruct it instead.\n";
    }

    start = file ;

    if(file[0..<3] == VOID_OB) {
        return "Error [update]: You cannot update the void object.\n";
    }

    if(obj = find_object(file)) {
        users = filter_array(all_inventory(obj), (: interactive :));
        users->move(load_object(VOID_OB), 1);
    }

    if(!file_exists(file))
        return "Error [update]: " + file  + " does not exist.\n";

    caller->set("cwf", file);

    obj = load_object(file);
    files = ({ file_name(obj) }) ;
    if(depth == 1) files += inherit_list(obj) ;
    else if(depth == 2) files += deep_inherit_list(obj) ;
    files = map(files, (: append($1, ".c") :)) ;
    filter(files, (: do_update :)) ;

    obj = load_object(file) ;
    if(pointerp(users)) users->move(obj, 1);
    return 1 ;

    error = catch(obj = load_object(file));


    if(error){
        write("Error [update]: Failed to load: "+file+"\nError: "+error+"\n");
        return 1;
    }

    return "Successful [update]: " +  file+" was updated.\n" ;
}

void do_update(string file) {
    object obj;
    string error, *users;
    string *files ;

    if(obj = find_object(file)) {
        obj->remove() ;
        if(obj) destruct(obj);
    }

    if(!file_exists(file))
        write("Error [update]: " + file  + " does not exist.\n") ;

    error = catch(obj = load_object(file));

    if(error){
        write("Error [update]: Failed to load: "+file+"\nError: "+error+"\n");
        return ;
    }

    if(pointerp(users)) users->move(obj, 1);
    write("Successful [update]: " + file + " was updated.\n") ;
}

string help(object caller) {
    return
"SYNTAX: update [-rR] [<file|here>]\n\n" +
"If file is not specified, the current working file will be updated. If the "
"file is specified, it will be updated. If the -r or -R flag is specified, "
"the file will be updated along with all of its inherited files. The -r flag "
"will only update the first level of inheritance, while the -R flag will "
"update all levels of inheritance.\n\n"
"Examples:\n"
"update - Update the cwf.\n"
"update here - Update the current room.\n"
"update -r here - Update the current room and its inherit_list().\n"
"update -R here - Update the current room and its deep_inherit_list().\n"
"update /cmds/std/who - Update the who command.\n"
"update -r /cmds/std/who - Update the who command and its inherit_list().\n"
"update -R /cmds/std/who - Update the who command and its deep_inherit_list().\n"
"update -r - Update the cwf and its inherit_list().\n"
"update -R - Update the cwf and its deep_inherit_list().\n" ;
}
