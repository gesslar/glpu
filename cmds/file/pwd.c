//pwd.c

//Tacitus @ LPUniversity
//08-APR-05
//File system management

inherit STD_CMD ;

void setup() {
    help_text =
"Syntax: pwd\n\n"
"This command will inform you of your current working directory and current "
"working file. This is important, as sometimes if you don't supply arguments "
"to commands, they default to your current working file or directory (such "
"as ls, ed, or update)." ;
}

mixed main(object caller, string arg) {
    write("Current working directory: " + caller->query_env("cwd") + "\n");
    write("Current working file: " + caller->query_env("cwf") + "\n");
    return 1;
}
