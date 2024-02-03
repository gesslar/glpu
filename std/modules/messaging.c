
int _contents_can_hear, _environment_can_hear;

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

varargs receive_inside_message(string msg, object *exclude, int msg_type) {
    object *contents;
    int i;

    do_receive(msg, msg_type) ;

    if(!living()) {
        if(query_environment_can_hear()) {
            environment()->receive_inside_message(msg, exclude, msg_type);
        }
    }
}

varargs receive_outside_message(string msg, object *exclude, int msg_type) {
    object *contents;
    int i;

    do_receive(msg, msg_type);

    if(!living()) {
        if(environment()) {
            if(query_contents_can_hear()) {
                contents = all_inventory(environment());
                for(i = 0; i < sizeof(contents); i++) {
                    if(member_array(contents[i], exclude) == -1) {
                        contents[i]->receive_outside_message(msg, exclude, msg_type);
                    }
                }
            }
        }
    }
}

varargs receive_private_message(string msg, int message_type, mixed other) {
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
