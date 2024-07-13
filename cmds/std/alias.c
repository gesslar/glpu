/* alias.c

 Tacitus @ LPUniversity
 14-JAN-06
 Alias command

*/

inherit STD_CMD ;

mixed main(object caller, string args) {
    string verb, alias;
    string *out = ({ }) ;

    if(!args) {
        mapping data = caller->get_aliases(0);
        string *keys;
        int i ;
        int sz ;

        if(!mapp(data))
            return "Aliases: No local aliases defined.\n" ;

        keys = keys(data);
        sz = sizeof(keys);

        if(!sz)
            return "Aliases: No local aliases defined.\n" ;

        out = allocate(sz + 1);
        out[0] = "Current local aliases:";

        for(i = 0; i < sz; i++) {
            out[i+1] = sprintf("%-10s %-20s", keys[i], data[keys[i]]) ;
        }

        out += ({ "", sprintf("Number of local aliases: %d", sz) }) ;

        return out ;
    }

     if(args == "-g") {
        mapping data = caller->get_aliases(1);
        string *keys;
        int i, sz;

        if(!mapp(data))
            return "Aliases: No global aliases defined.\n" ;

        keys = keys(data);
        sz = sizeof(keys);

        if(!sz)
            return "Aliases: No global aliases defined.\n" ;

        out = allocate(sz + 1);
        out[0] = "Current global aliases:";

        for(i = 0; i < sz; i++) {
            out[i+1] = sprintf("%-10s %-20s", keys[i], data[keys[i]]) ;
        }

        out += ({ "", sprintf("Number of global aliases: %d", sz) }) ;

        return out ;
    }

    if(sscanf(args, "%s %s", verb, alias) == 2) {
        if(strsrch(alias, "$*") == -1) {
            if(alias[<1] != ' ') alias += " $*";
            else alias += "$*";
        }

        if(verb == "alias" || verb == "unalias")
            return "Error: You may not alias 'alias' or 'unalias'.\n";

        caller->add_alias(verb, alias);

        return "Alias: Added alias '" + verb + " " + alias + "'\n" ;
    }

    return "Syntax: alias [-g] <[$]verb> <alias>\n";
}

string help(object caller) {
    return(" SYNTAX: alias [-g] <[$]verb> <alias>\n\n"
    "This command allows you to display your current aliases and\n"
    "add new ones. If you would like to view your current aliases,\n"
    "then simply type 'alias'. If you'd like to see your current\n"
    "aliases plus global aliases then affect you, provie -g as an\n"
    "argument by it's self. To add an alias, you must provide two arguments;\n"
    "the verb you wish to alias and the actual alias. You may use $n,\n"
    "where represents a number that repsents the index of a word as an\n"
    "argument when the alias is parsed. (example: 'alias say say %2 %1'\n"
    "will result in the first and second word to be always switched when\n"
    "you type say). You can also use $*, which will be replaced with all\n"
    "remaining words. If you place $ in front of the verb, then all commands\n"
    "beginning with that verb will be parsed with that alias\n"
    "(ex. 'alias $: emote $*' will result in any command beginning with ':' to\n"
    "be parsed as emote $*).\n\n"
    "See also: unalias\n");
}
