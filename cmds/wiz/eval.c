//eval.c

//Tacitus @ LPUniversity
//06-APR-05
//Wiz Tool

//Needs to be redone

//Last updated October 6th, 2006 by Tacitus

inherit CMD ;

int main(object caller, object room, string arg)
{
     /* clean up first */
     string err;
     if(!directory_exists(user_path(caller->query_name())))
          return(notify_fail("Error [eval]: You must have a home directory to use eval.\n"));
     write("Evaluating: " + arg + "\n\n");
     if (file_size(user_path(caller->query_name()) + "tmp_eval_file.c") != -1)
     rm (user_path(caller->query_name()) + "tmp_eval_file.c");
     if (find_object(user_path(caller->query_name()) + "tmp_eval_file"))
          destruct(find_object(user_path(caller->query_name()) + "tmp_eval_file"));

     write_file(user_path(caller->query_name()) + "tmp_eval_file.c","mixed eval() { "+arg+"; }\n");
     err = catch(printf("Result = %O\n", load_object(user_path(caller->query_name()) + "tmp_eval_file.c")->eval()));
     if(err) write("\nRuntime error:\n " + err + "\nSee logs for more details.\n");
     rm(user_path(caller->query_name()) + "tmp_eval_file.c");
     return 1;
}

string help(object caller)
{
     return("SYNTAX: eval <lpc-statements>\n\n" +
     "This command allows you to execute stand-alone lpc statements.\n"
     "This is considered a more advanced tool and abuse of it is not\n"
     "recommended. Also note that you must have a home directory.\n");
}
