// Functions
void do_receive(string message, int message_type);

// Variables
int _contents_can_hear = 1, _environment_can_hear = 1;

// This function enables messages to be propogated to the contents of an object
int set_contents_can_hear(int i) {
    _contents_can_hear = !!i;
    return i;
}

// This function enables messages to be propogated to the environment of an object
int set_environment_can_hear(int i) {
    _environment_can_hear = !!i;
    return i;
}

// This function returns the value of the _can_hear_contents variable
int query_contents_can_hear() {
    return _contents_can_hear;
}

// This function returns the value of the _can_hear_environment variable
int query_environment_can_hear() {
    return _environment_can_hear;
}

// tell_up() is a function that sends a message to the environment of an object
// and all of the contents of that object, excluding the object passed as the
// second argument.
varargs void receive_up(string msg, object *exclude, int msg_type) {
    object env;
    object *contents;
    int i;

    do_receive(msg, msg_type);

    if(!query_environment_can_hear()) return ;

    if(objectp(exclude)) exclude = ({ exclude });
    if(!pointerp(exclude)) exclude = ({});
    exclude += ({ this_object() });

    env = environment() ;
    if(env) {
        contents = all_inventory(env);
        contents -= exclude;
        contents->receive_up(msg, exclude, msg_type);

        if(member_array(env, exclude) == -1) {
            exclude += ({ env });
            env->receive_up(msg, exclude, msg_type);
        }
    }
}

// tell_down() is a function that sends a message to all of the contents of an
// object, excluding the object passed as the second argument.
varargs void receive_down(string msg, object *exclude, int msg_type) {
    object *contents;
    int i;

    do_receive(msg, msg_type);

    if(!query_contents_can_hear()) return ;

    if(objectp(exclude)) exclude = ({ exclude });
    if(!pointerp(exclude)) exclude = ({});

    contents = all_inventory();
    contents -= exclude;
    contents->receive_down(msg, exclude, msg_type);
}

// tell_all() is a function that sends a message to the environment of an object
// and all of the contents of that object, excluding the object passed as the
// second argument.
varargs void receive_all(string msg, object *exclude, int msg_type) {
    object env;
    object *contents;
    int i;

    do_receive(msg, msg_type);

    if(!query_environment_can_hear()) return ;
    if(!query_contents_can_hear()) return ;

    if(objectp(exclude)) exclude = ({ exclude });
    if(!pointerp(exclude)) exclude = ({});
    exclude += ({ this_object() });

    env = environment() ;
    if(env) {
        contents = all_inventory(env);
        contents -= exclude;
        contents->receive_all(msg, exclude, msg_type);

        if(member_array(env, exclude) == -1) {
            exclude += ({ env });
            env->receive_all(msg, exclude, msg_type);
        }
    }

    contents = all_inventory();
    contents -= exclude;
    contents->receive_all(msg, exclude, msg_type);
}

varargs receive_direct(string msg, int message_type, mixed other) {
    do_receive(msg, message_type);
}

void do_receive(string message, int message_type) {
    string term ;

    if(userp(this_object())) {
        term = this_object()->query_env("colour");
        // If colour is not explicitly enabled, set NO_ANSI to disable coloured messages.
        if(term == "enabled") {
            term = "xterm" ;
        } else {
            term = "plain";
            message_type |= NO_ANSI;
        }
    } else {
        // For non-user objects, also disable coloured messages.
        message_type |= NO_ANSI;
    }

    // If NO_ANSI flag is set, substitute colours with "plain" (i.e., no colour).
    if(message_type & NO_ANSI) {
        message = XTERM256->substitute_colour(message, "plain");
    }
    else {
        message = XTERM256->substitute_colour(message, term);
    }

    if(!(message_type & MSG_PROMPT)) {
        message = append(message, "\n") ;
    }

    receive(message) ;

    // Handle telnet go-ahead if it's a prompt.
    if(message_type & MSG_PROMPT) telnet_ga();
}
