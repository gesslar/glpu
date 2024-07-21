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

        input_to("_password", I_NOECHO | I_NOESC, body, attempts, count, cb) ;
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

        input_to("_password", I_NOECHO | I_NOESC, body, attempts, count, cb) ;
        return ;
    }
    _ok(body, "Password accepted.") ;
    call_back(cb, 1) ;
}

/**
 * @simul_efun prompt_password
 * @description Prompt the user for a password.
 * @param {object} body - The user's body object.
 * @param {int} attempts - The number of attempts allowed.
 * @param {mixed*} cb - The callback array.
 * @example prompt_password(this_body(), 3, assemble_callback(this_object(), "password_callback", this_body())) ;
 */
void prompt_password(object body, int attempts, mixed *cb) {
    _question(body, "Password: ") ;
    input_to("_password", I_NOECHO | I_NOESC, body, attempts, 1, cb) ;
}

/**
 * @simul_efun prompt_colour
 * @description Prompt the user for a colour. The user will be presented with a
 *              list of colours to choose from. The user can also enter a
 *              number corresponding to an xterm 256 colour, or the word "plain"
 *              to select no colour.
 *
 *              Upon selection, the callback function will be called with the
 *              selected colour as the argument.
 * @param {object} body - The user's body object.
 * @param {mixed*} cb - The callback array.
 * @param {string} prompt - The prompt message.
 * @example prompt_colour(this_body(), assemble_callback(this_object(), "colour_callback", this_body()), "Select a colour from the following list:") ;
 */
varargs void prompt_colour(object body, mixed *cb, string prompt) {
    string mess ;

    if(nullp(body) || !userp(body))
        error("Bad argument 1 to prompt_colour(): expected body object, got " + typeof(body) + ".") ;

    if(nullp(cb) || !pointerp(cb))
        error("Bad argument 2 to prompt_colour(): expected callback array, got " + typeof(cb) + ".") ;

    if(stringp(prompt))
        mess = prompt ;
    else
        mess = "Select a colour from the following list:" ;

    mess = append(mess, "\n") ;
    mess += "\n" ;

    mess +=
        "\e0000\eblack\e\eres\e\t\t"
        "\e0001\ered\e\eres\e\t\t"
        "\e0002\egreen\e\eres\e\t\t"
        "\e0003\eorange\eres\e\n"
        "\e0004\eblue\e\eres\e\t\t"
        "\e0005\emagenta\e\eres\e\t\t"
        "\e0006\ecyan\eres\e\t\t"
        "\e0007\ewhite\e\eres\e\n"
        "\n"
        "\e0008\ebright black\e\eres\e\t"
        "\e0009\ebright red\e\eres\e\t"
        "\e0010\ebright green\eres\e\t"
        "\e0011\ebright orange\e\eres\e\n"
        "\e0012\ebright blue\e\eres\e\t"
        "\e0013\ebright magenta\eres\e\t"
        "\e0014\ebright cyan\e\eres\e\t"
        "\e0015\ebright white\e\eres\e\n" ;

    mess += "\n" ;

    mess += "Select one of (color), (0-255), (c)olour list, (p)lain, or (q)uit.\n"
            "Your choice: " ;

    tell(body, mess) ;
    input_to("_prompt_colour", I_NOESC, mess, body, cb) ;
}

private nosave mapping _colour_list = ([
    "black"         : "000",
    "red"           : "001",
    "green"         : "002",
    "orange"        : "003",
    "blue"          : "004",
    "magenta"       : "005",
    "cyan"          : "006",
    "white"         : "007",
    "bright black"  : "008",
    "bright red"    : "009",
    "bright green"  : "010",
    "bright orange" : "011",
    "bright blue"   : "012",
    "bright magenta": "013",
    "bright cyan"   : "014",
    "bright white"  : "015"
]) ;

private void _prompt_colour(string input, string prompt, object body, mixed *cb) {
    string *colours ;
    int i ;

    if(!input || input == "") {
        _error(body, "Invalid selection.") ;
        tell(body, prompt) ;
        input_to("_prompt_colour", I_NOESC, prompt, body, cb) ;
        return ;
    }

    if(input == "c") {
        string list = COLOUR_D->get_colour_list() ;
        prompt = "Select a colour from the following list:\n" ;
        prompt += "\n" ;
        prompt += list ;
        prompt += "\n" ;
        prompt += "Select (0-255), (p)lain), or (q)uit.\n" ;
        prompt += "Your choice: " ;

        tell(body, prompt) ;

        input_to("_prompt_256", 0, body, cb, prompt) ;
        return ;
    }

    if(input == "p" || input == "plain") {
        call_back(cb, "plain") ;
        return ;
    }

    if(input == "q" || input == "quit") {
        call_back(cb, "cancel") ;
        return ;
    }

    i = to_int(input) ;
    if(!nullp(i)) {
        if(i < 0 || i > 255) {
            _error(body, "Invalid selection.") ;
            tell(body, prompt) ;
            input_to("_prompt_colour", I_NOESC, prompt, body, cb) ;
            return ;
        }
    } else {
        if(of(input, _colour_list)) {
            input = _colour_list[input] ;
        } else {
            _error(body, "Invalid selection.") ;
            tell(body, prompt) ;
            input_to("_prompt_colour", I_NOESC, prompt, body, cb) ;
            return ;
        }
    }

    call_back(cb, input) ;
}

private void _prompt_256(string input, object body, mixed *cb, string prompt) {
    string *colours ;
    int i ;

    if(!input || input == "") {
        _error(body, "Invalid selection.") ;
        tell(body, prompt) ;
        input_to("_prompt_256", I_NOESC, body, cb, prompt) ;
        return ;
    }

    if(input == "plain") {
        body->set_env("prompt_colour", 0) ;
        call_back(cb, input) ;
        return ;
    }

    if(input == "q" || input == "quit") {
        call_back(cb, "cancel") ;
        return ;
    }

    i = to_int(input) ;

    if(i < 0 || i > 255) {
        _error(body, "Invalid selection.") ;
        tell(body, prompt) ;
        input_to("_prompt_256", I_NOESC, body, cb, prompt) ;
        return ;
    }

    call_back(cb, sprintf("%03d", i)) ;
}
