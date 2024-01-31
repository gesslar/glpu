//say.c

//Tacitus @ LPUniversity
//01-JULY-05
//Simple communication command

inherit CMD ;

int main(object caller, object room, string arg) {
    if(!arg) {
        write("You mumble to your self.\n");
        say(capitalize((string)previous_object()->query_name()) + " mumbles to themself.\n");
        return 1;
    } else {
        write("You say: " + arg + "\n");
        say(capitalize((string)previous_object()->query_name())
            + " says: " +  arg + "\n");
        return 1;
    }
}

string help(object caller) {
    return(" SYNTAX: say <argument>\n\n" +
      "This command will send a message to everyone in the room.\n" +
      "Example, if you type 'say Hello!' then the others in the\n" +
      "room will see '" + capitalize(caller->query_name()) + " says: Hello!'\n\n" +
      "See also: tell, tune\n");
}
