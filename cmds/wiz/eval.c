//eval.c

//Tacitus @ LPUniversity
//06-APR-05
//Wiz Tool

//Needs to be redone

//Last updated October 6th, 2006 by Tacitus

inherit STD_CMD ;

mixed main(object caller, object room, string arg) {
    string file ;
    object ob ;
    string err ;
    mixed result ;

    if(!directory_exists(user_path(caller->query_proper_name())))
        return "Error [eval]: You must have a home directory to use eval.\n" ;

    tell(caller, "Evaluating: " + arg + "\n\n");
    file = user_path(caller->query_proper_name()) + "tmp_eval_file.c" ;
    if(file_size(file) != -1)
        rm(file) ;

    if(ob = find_object(file))
        destruct(ob) ;

    write_file(file,"mixed eval() { "+arg+"; }\n");
    if(err = catch(ob = load_object(file))) {
        tell(caller, "\nError loading file " + file + "\n"+err+"\n");
        rm(file);
        return 1 ;
    }

    if(err = catch(result = ob->eval())) {
        tell(caller, "\nRuntime error:\n " + err + "\nSee logs for more details.\n");
        rm(file);
        return 1 ;
    }

    tell(caller, sprintf("Result = %O\n", result));
    destruct(ob);
    rm(file);
    return 1 ;
}

string help(object caller) {
     return("SYNTAX: eval <lpc-statements>\n\n" +
     "This command allows you to execute stand-alone lpc statements.\n"
     "This is considered a more advanced tool and abuse of it is not\n"
     "recommended. Also note that you must have a home directory.\n");
}
