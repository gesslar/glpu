/* unalias.c

 Tacitus @ LPUniveristy
 14-JAN-05
 Unalias command

*/

inherit STD_CMD ;

mixed main(object caller, string args) {
     if(!args) return "Syntax: unalias <verb>\n";

     if(caller->remove_alias(args))
          return "Success [aliases]: Verb '" + args + "' is no longer aliased.\n";

     return "Error [aliases]: That verb is not aliased locally.\n";
}

string help(object caller) {
     return("SYNTAX: unalias <verb>\n\n"
     "This command allows you to unalias local aliases. A local alias\n"
     "is an alias that you have set, not global aliases. To unalias\n"
     "a verb that is currently aliased locally, simply provide the\n"
     "verb as the argument. If you don't like a global alias, you can\n"
     "create an alias with the same verb to override it.\n\n"
     "See also: alias\n");
}
