#include "/adm/obj/simul_efun.h"

private nomask void _password(string input, object body, int attempts, int count, mixed *cb) {
    string current_password ;
    if(!input || input == "") {
        _error(body, "Password cannot be empty.") ;
        if(++count > attempts) {
            call_back(cb, 0) ;
            return ;
        }

        if(attempts > 1)
            _question(body, "Password (attempt %d): ", count) ;
        else
            _question(body, "Password: ") ;

        input_to("_password", INPUT_NOECHO | INPUT_IGNORE_BANG, body, attempts, count, cb) ;
        return ;
    }
    current_password = body->query_user()->query_password() ;
    if(crypt(input, current_password) != current_password) {
        _error(body, "Incorrect password.") ;
        if(++count > attempts) {
            call_back(cb, 0) ;
            return ;
        }

        if(attempts > 1)
            _question(body, "Password (attempt %d): ", count) ;
        else
            _question(body, "Password: ") ;

        input_to("_password", INPUT_NOECHO | INPUT_IGNORE_BANG, body, attempts, count, cb) ;
        return ;
    }
    _ok(body, "Password accepted.") ;
    call_back(cb, 1) ;
}

void prompt_password(object body, int attempts, mixed *cb) {
    _question(body, "Password: ") ;
    input_to("_password", INPUT_NOECHO | INPUT_IGNORE_BANG, body, attempts, 1, cb) ;
}
