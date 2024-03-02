// /std/modules/init.c
// The standard module for using the init() apply
//
// Created:     2024/02/29: Gesslar
// Last Change: 2024/02/29: Gesslar
//
// 2024/02/29: Gesslar - Created

private nosave mapping commands = ([]);
private nosave mixed *inits = ({});

void add_command(string cmd, string fun) {
    commands[cmd] = fun;
}

void add_init(mixed fun) {
    inits += ({ fun });
}

void init() {
    int sz ;
    object tp ;

    foreach (mixed cmd, string fun in commands) {
        add_action(fun, cmd);
    }

    sz = sizeof(inits);
    for (int i = 0; i < sz; i++) {
        if (valid_function(inits[i])) {
            (*inits[i])(tp) ;
        } else {
            catch( call_other(this_object(), inits[i], tp) );
        }
    }
}
