/* alias.c

 Tacitus @ LPUniversity
 14-JAN-06
 Alias command

*/

inherit STD_CMD ;

mixed list_aliases(object tp, int global) ;

mixed main(object tp, string args) {
    string verb, alias ;
    mapping aliases ;

    if(!args) return list_aliases(tp, 0) ;
    else if(args == "-g") return list_aliases(tp, 1) ;

    if(sscanf(args, "%s %s", verb, alias) == 2) {
        if(verb == "alias" || verb == "unalias")
            return _error("You may not alias 'alias' or 'unalias'.") ;

        tp->add_alias(verb, alias) ;

        return _ok("Added alias `%s`\n>> %s", verb, alias) ;
    }

    aliases = tp->get_aliases(0) ;
    if(of(args, aliases)) {
        alias = aliases[args] ;
        return _info("%s is aliased to:\n>> %s", args, alias) ;
    }
    aliases = tp->get_aliases(1) ;
    if(of(args, aliases)) {
        alias = aliases[args] ;
        return _info("%s is globally aliased to:\n>> %s", args, alias) ;
    }

    return _error("No such alias `%s` defined.", args) ;
}

mixed list_aliases(object tp, int global) {
    mapping data = tp->get_aliases(global) ;
    string *keys, header, footer ;
    int i, sz ;
    string *out ;

    if(!mapp(data))
        return "Aliases: No aliases defined.\n" ;

    if(global) {
        header = "Current global aliases:" ;
        footer = "Number of global aliases: %d" ;
    } else {
        header = "Current local aliases:" ;
        footer = "Number of local aliases: %d" ;
    }

    keys = keys(data) ;
    sz = sizeof(keys) ;

    if(!sz)
        return "Aliases: No aliases defined.\n" ;

    keys = sort_array(keys, 1) ;

    out = allocate(sz + 4) ;
    out[0] = header ;
    out[1] = "" ;
    out[<2] = "" ;
    out[<1] = sprintf(footer, sz) ;

    for(i = 0; i < sz; i++) {
        out[i+2] = sprintf("%-10s %-20s", keys[i], data[keys[i]]) ;
    }

    return out ;
}

string help(object tp) {
    return(" SYNTAX: alias [-g] <[$]verb> <alias>\n\n"
    "This command allows you to display your current aliases and\n"
    "add new ones. If you would like to view your current aliases,\n"
    "then simply type 'alias'. If you'd like to see your current\n"
    "aliases plus global aliases then affect you, provie -g as an\n"
    "argument by it's self. To add an alias, you must provide two arguments;\n"
    "the verb you wish to alias and the actual alias. You may use $n,\n"
    "where represents a number that represents the index of a word as an\n"
    "argument when the alias is parsed. (example: 'alias say say %2 %1'\n"
    "will result in the first and second word to be always switched when\n"
    "you type say). You can also use $*, which will be replaced with all\n"
    "remaining words. If you place $ in front of the verb, then all commands\n"
    "beginning with that verb will be parsed with that alias\n"
    "(ex. 'alias $: emote $*' will result in any command beginning with ':' to\n"
    "be parsed as emote $*).\n\n"
    "See also: unalias\n") ;
}
