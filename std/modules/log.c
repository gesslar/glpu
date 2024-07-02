protected nosave nomask int _log_level = 1 ;

void set_log_level(int lvl) {
    _log_level = lvl ;
}

int log_level() {
    return _log_level ;
}

varargs void _log(mixed args...) {
    int lvl = -1 ;
    string message ;
    mixed *rest ;
    string out ;

    if(!sizeof(args))
        return ;

    switch(sizeof(args)) {
        case 0:
            return ;
        case 1:
            lvl = 1 ;
            message = args[0] ;
            break ;
        default:
            if(stringp(args)) {
                lvl = 1 ;
                message = args ;
                rest = ({}) ;
            } else {
                if(stringp(args[0])) {
                    lvl = 1 ;
                    message = args[0] ;
                    rest = args[1..] ;
                } else if(intp(args[0]) && stringp(args[1]) && sizeof(args) >= 2) {
                    lvl = args[0] ;
                    message = args[1] ;
                    rest = args[2..] ;
                } else {
                    lvl = 1 ;
                    message = args[0] ;
                    rest = args[1..] ;
                }
            }
            break ;
    }

    if(lvl > _log_level)
        return ;

    while(message[<1] == '\n')
        message = message[0..<2] ;

    if(_log_level >= 2) {
        string *funcs = call_stack(2), func ;
        func = funcs[1] ;
        message = func + " - " + message ;
    }

    if(sizeof(rest))
        debug("# " + message, rest...) ;
    else
        debug("# " + message) ;
}
