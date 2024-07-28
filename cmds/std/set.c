//set_env.c

//Tacitus @ LPUniversity
//24-OCT-05
//Standard user

/* Last edited on October 4th, 2006 by Tacitus */

inherit STD_CMD ;

void prompt_colour_result(string input, object tp, string variable) ;

mixed main(object tp, string str) {
    string var_name, var_value, *keys;
    mapping data;
    int i;

    if(!str) {
        data = tp->list_env();
        if(!mapp(data))
            return _info("No environment values currently set.");
        else {
            if(!sizeof(data))
                return _info("No environment values currently set.");

            keys = keys(data);
            keys = sort_array(keys, 1);
            tell(tp, "Current environment variables:\n\n") ;

            for(i = 0; i < sizeof(keys); i ++)
                tell(tp,
                        sprintf("%-20s : %-20s\n", keys[i], data[keys[i]]),
                        NO_COLOUR
                ) ;

            return 1;
        }
    }

    sscanf(str, "%s %s", var_name, var_value);

    if(!var_name)
     var_name = str;

    if(strlen(var_name) > 20)
        return _error("Variable name too long.");

    if(pcre_match(var_name, "^[a-zA-Z0-9_]+_colour$") && var_value == "prompt") {
        prompt_colour(
            tp,
            assemble_call_back((: prompt_colour_result :), tp, var_name),
            "Select the colour you would like to set for '" + var_name + "':"
        );

        return 1;
    }

    tp->set_env(var_name, var_value);

    if(!var_value)
        return _ok(tp, "Value '" + var_name + "' cleared.");
    else
        return _ok(tp, "Value '" + var_name + "' set to '" + var_value + "'.");
}

void prompt_colour_result(string result, object tp, string variable) {
    switch(result) {
        case "cancel":
            _ok("You opt not to set this value at this time.") ;
            return;
        case "plain":
            tp->set_env(variable, "");
            _ok(tp, "Selected plain for %s.", variable);
            return;
        default: {
            string colour ;

            result = COLOUR_D->substitute_too_dark(result);
            colour = sprintf("\e0%s\e", result) ;
            tp->set_env(variable, result);
            _ok(tp, "You have selected %s%s\eres\e for `%s`.",
                colour,
                result,
                variable
            ) ;
            return;
        }
    }
}

string query_help(object tp) {
     return(
"SYNTAX: set [<var_name>] [<var_value>]\n\n"
"This command allows you, the user, to set environment values that effect the "
"way the mud interacts with you. If you do not provide an argument, then this "
"command will return information regarding your currently set environment "
"variables.\n"
"\n"
"To set a variable, you would type 'set var_name var_value'. If you omit "
"the second argument and a environmental variable with a name matching the "
"first argument exists, then that environmental variable will be deleted.\n\n"
"See also: environmental variables, colour\n");
}
