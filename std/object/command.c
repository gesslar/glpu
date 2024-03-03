// /std/object/command.c
// Replacement for add_action, in order to allow for mixed results.
//
// Created:     2024/03/03: Gesslar
// Last Change: 2024/03/03: Gesslar
//
// 2024/03/03: Gesslar - Created

private nosave mapping commands = ([]);

int add_command(mixed command, mixed action) ;
void remove_command(mixed command) ;
void remove_command_all(mixed action) ;
mixed query_command(string command) ;
mapping query_commands() ;
string *query_matching_commands(string command) ;
void init_commands() ;
mixed evaluate_command(object tp, string command, string arg) ;

void add_command(mixed command, mixed action) {
    if (stringp(command)) {
        remove_command(command) ;
        if (stringp(action)) {
            if(!function_exists(action)) {
                error("add_command: No such function " + action + " in " + file_name(this_object()) + ".\n");
            }
            commands[command] = action;
        } else if (valid_function(action)) {
            commands[command] = action;
        } else {
            error("add_command: Illegal action " + action + " in " + file_name(this_object()) + ".\n");
        }
    } else if (pointerp(command)) {
        foreach (mixed cmd in command) {
            add_command(cmd, action) ;
        }
    } else {
        error("add_command: Illegal command " + command + " in " + file_name(this_object()) + ".\n");
    }
}

void remove_command(mixed command) {
    mixed action ;

    if (stringp(command)) {
        action = commands[command] ;
        map_delete(commands, command);
    } else if (pointerp(command)) {
        foreach (mixed cmd in command) {
            if (stringp(cmd)) {
                action = commands[cmd] ;
                map_delete(commands, cmd);
            }
        }
    }
}

void remove_command_all(mixed action) {
    foreach (mixed command, mixed act in commands) {
        if (act == action) {
            map_delete(commands, command);
            remove_command(command) ;
        }
    }
}

mixed query_command(string command) {
    return commands[command];
}

mapping query_commands() {
    return copy(commands);
}

string *query_matching_commands(string command) {
    string *matches = allocate(1) ;

    if(nullp(commands[command])) {
        return ({});
    }

    matches[0] = command ;

    foreach (mixed cmd, mixed action in commands) {
        if(cmd == matches[0]) continue ;
        if(action == commands[command]) {
            matches += ({ cmd }) ;
        }
    }

    return matches ;
}

void init_commands() {
    commands = ([]);
}

mixed evaluate_command(object tp, string command, string arg) {
    mixed action = commands[command] ;

    if (stringp(action)) {
        return call_other(this_object(), action, tp, arg);
    } else if (valid_function(action)) {
        return (*action)(tp, arg);
    }

    return null ;
}
