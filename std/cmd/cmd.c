// /std/cmd/cmd.c
// Base command object
//
// Created:     2022/08/24: Gesslar
// Last Change: 2022/08/24: Gesslar
//
// 2022/08/24: Gesslar - Created

inherit STD_OBJECT ;
inherit M_SETUP ;

// Functions
string help(object caller) ;

// Variables
protected mixed help = (: help :) ;

// Private so only the driver can call it.
private void create() {
    setup_chain() ;
}

string query_help(object caller) {
    string temp ;
    if(valid_function(help))
        temp = (*help)(caller) ;
    else if(stringp(help))
        temp = help ;
    else
        temp = "There is no help on this topic." ;

    return temp ;
}

int is_command() {
    return 1 ;
}
