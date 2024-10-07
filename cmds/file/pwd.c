//pwd.c

//Tacitus @ LPUniversity
//08-APR-05
//File system management

inherit STD_CMD ;

void setup() {
    usage_text = "pwd" ;
    help_text =
"This command will inform you of your current working directory and current "
"working file. This is important, as sometimes if you don't supply arguments "
"to commands, they default to your current working file or directory (such "
"as ls, ed, or update)." ;
}

mixed main(object caller, string arg) {
    printf("Current working directory: %s\n", caller->query_env("cwd")) ;
    printf("Current working file: %s\n", caller->query_env("cwf")) ;
    return 1 ;
}
