//eval.c

//Tacitus @ LPUniversity
//06-APR-05
//Wiz Tool

//Needs to be redone

//Last updated October 6th, 2006 by Tacitus

inherit STD_CMD ;

mixed main(object caller, string arg) {
    string file ;
    object ob ;
    string err ;
    mixed result ;
    int limit = max_eval_cost(), cost ;

    if(!directory_exists(user_path(caller->query_real_name())))
        return _error("You must have a home directory to use eval.") ;

    _info(caller, "Evaluating: %s", arg);
    file = user_path(caller->query_real_name()) + "tmp_eval_file.c" ;
    if(file_size(file) != -1)
        rm(file) ;

    if(ob = find_object(file))
        destruct(ob) ;

    write_file(file,"mixed eval() { "+arg+"; }\n");
    if(err = catch(ob = load_object(file))) {
        rm(file);
        return _error("Error loading file %s\n%s", file, err) ;
    }


    err = catch {
        reset_eval_cost() ;
        result = ob->eval() ;
        cost = limit - eval_cost() ;
    } ;

    catch(destruct(ob)) ;
    rm(file) ;

    if(err)
        return _error("Runtime error:\n%s\nSee logs for more details.", err) ;

    _ok("Result = %O", result) ;
    _ok("Eval cost: %s", add_commas(cost)) ;

    return 1 ;
}

string help(object caller) {
     return("SYNTAX: eval <lpc-statements>\n\n" +
     "This command allows you to execute stand-alone lpc statements.\n"
     "This is considered a more advanced tool and abuse of it is not\n"
     "recommended. Also note that you must have a home directory.\n");
}
