// tail.c

// Gwegster @ LPUniversity
// 30-JULY-05
// A simple tail command

//Last edited on July 1st, 2005 by Tacitus

inherit STD_CMD ;

int main(object caller, object room, string file)
{
     if(!file)
     {
          write("Usage: tail <filename>\n");
          return 1;
     }

     file = resolve_path(caller->query("cwd"), file);

     if(directory_exists(file))
     {
          write("Error [tail]: File specified is a directory.\n");
          return 1;
     }

     if(!file_exists(file))
     {
          write("Error [tail]: That file does not exist.\n");
          return 1;
     }

     if(!caller->query("morelines"))
     {
          caller->set("morelines", 20);
     }

     return tail(file);
}

string help(object caller) {
    return (" SYNTAX: tail <filename>\n\n" +
    "This command prints the last 10 lines of a specified file.\n");
 }
