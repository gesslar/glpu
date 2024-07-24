// /std/cmd/cmd.c
// Base command object
//
// Created:     2022/08/24: Gesslar
// Last Change: 2022/08/24: Gesslar
//
// 2022/08/24: Gesslar - Created

inherit STD_OBJECT ;
inherit M_SETUP ;
inherit M_TARGETING ;

// Functions
string help(object caller) ;
string usage(object caller) ;

// Variables
protected mixed help_text = (: help :) ;
protected mixed usage_text = (: usage :) ;

// Private so only the driver can call it.
private void create() {
    setup_chain() ;
}

string query_help(object caller) {
    string result ;
    string temp ;

    if(valid_function(help_text))
        temp = evaluate(help_text, caller) ;
    else if(stringp(help_text))
        temp = help_text ;
    else
        return "There is no help available on this topic." ;

    result = append(temp, "\n") ;

    if(valid_function(usage_text))
        temp = evaluate(temp, caller) ;
    else if(stringp(usage_text))
        temp = usage_text ;

    if(temp)
        result = "Usage:\n"+append(temp, "\n") + "\n" + result ;

    while(result[<1] == '\n')
        result = result[0..<2] ;

    return result ;
}

int is_command() {
    return 1 ;
}

string usage (object caller) { return null ; }
