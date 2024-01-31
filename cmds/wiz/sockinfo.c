/* sockinfo.c

 Tacitus @ LPUniversity
 05-MAY-06
 Socket info command

*/

inherit CMD ;

int main(){
      write(dump_socket_status());
      return 1;
}

string help(object caller) {
     return(" SYNTAX: sockinfo\n\n"
     "The sockinfo command will tell you what sockets are loaded\n"
     "and what they are currently being used for.\n");
}
