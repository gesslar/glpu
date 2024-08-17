/* reply.c

 Tacitus @ LPUniversity
 05-MAY-06
 Std reply command

*/

//Last edited on August 11th, 2006 by Parthenon
//Last edited on September 10th, 2006 by Tricky
//Last edited on September 18th, 2006 by Tricky

inherit STD_CMD ;

void setup() {
    usage_text = "reply <message>" ;
    help_text =
"This command will send a tell to the last person who sent you a tell. If "
"they are online, they will receive the message." ;
}

mixed main(object tp, string message) {
    object target ;
    string who = tp->query_tell_reply() ;
    string name, away, tname ;

    if(!who || !strlen(who))
        return "Nobody to reply to." ;

    if(!message)
        return "Reply with what?" ;

    target = find_player(who) ;

    if(!objectp(target)) {
        tp->set_tell_reply("") ;
        return "You cannot seem to find " + capitalize(who) + ".";
    }

    if(target == tp) {
        tp->simple_action("$N $vstart talking to $r.") ;
        return 1;
    }

    away = target->query_env("away") ;
    if(away) {
        return "That user is currently away" + (away != "" ?
            ": (" + away + ")\n" : ".\n") ;
    }

    name = tp->query_name() ;
    tname = target->query_name() ;

    if(message[0] == ':') {
        tell(tp, sprintf("From afar, you %s\n", message[1..])) ;
        tell(target, sprintf("From afar, %s %s\n", tname, message[1..])) ;
    } else {
        tell(tp, sprintf("You tell %s: %s\n", tname, message)) ;
        tell(target, sprintf("%s tells you: %s\n", name, message)) ;
    }

    target->set_tell_reply(query_privs(tp));

    return 1;
}
