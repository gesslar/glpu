//cat.c

//Tacitus @ LPUniveristy
//02-OCT-05
//Pages entire file at once

inherit STD_CMD ;

int main(object caller, object room, string str)
{
     string *lines;
     int i;

     if(!str) {
          write("Error [cat]: No argument passed.\n");
          return 1;
     }

     str=resolve_path(caller->query("cwd"),str);

     if(!file_exists(str))
     {
          write("Error [cat]: That file does not exist.\n");
          return 1;
     }

     lines = explode(read_file(str), "\n");

     for(i = 0; i < sizeof(lines); i++) write(lines[i] + "\n");
     write("\n");

     return 1;
}

string help(object caller) {
     return (" SYNTAX: cat <file>\n\n" +
    "This command pages through an entire file at once.\n");

}
