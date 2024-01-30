//mkdir.c

//Tacitus @ LPUniversity
//07-APR-05
//File system management

//Last edited on July 1st, 2005 by Tacitus

inherit CMD ;

int main(object caller, object room, string str)
{
     if(!str) return notify_fail("Syntax: mkdir <directory name>\n");
     str = resolve_path(caller->query("cwd"), str);
     if(file_exists(str)) return notify_fail("Error [mkdir]: " + str + " already a file.\n");
     if(directory_exists(str)) return notify_fail("Error [mkdir]: " + str + " already a directory.\n");
     if(!(int)master()->valid_write(str, caller, "mkdir"))
     {
          write("Error [mkdir]: Permission denied.\n");
          return 1;
     }
     write(mkdir(str) ? "Successful [mkdir]: " + str + " created.\n" : "Error [mkdir]: Failed to create " + str + ".\n");
     return 1;
}



string help(object caller) {
    return (HIW + " SYNTAX: " + NOR + "mkdir <new directory>\n\n" +
    "This command makes a new directory with the name you specified.\n");
}
