// /adm/daemons/bank.c
// This daemon handles banking activity using SQLITE3 and the DB_D daemon.
//
// Created:     2024/02/28: Gesslar
// Last Change: 2024/02/28: Gesslar
//
// 2024/02/28: Gesslar - Created

inherit STD_DAEMON ;

mixed new_account(string name) ;
int query_balance(string name) ;

private nosave string db = "bank" ;
// These statements are in sprintf format
private nosave mapping balance_statements = ([]) ;
private nosave mapping activity_statements = ([]) ;

void setup() {
    balance_statements = ([
        "select" : "SELECT amount FROM balance WHERE name = '%s' ;",
        "update" : "UPDATE balance SET amount = %d, time = %d WHERE name = '%s' ;",
        "add" : "INSERT INTO balance (name, amount, time) VALUES ('%s', %d, %d) ;",
    ]) ;
    activity_statements = ([
        "select": "SELECT * FROM activity WHERE name = '%s' ORDER BY time DESC ;",
        "select_limited": "SELECT * FROM activity WHERE name = '%s' ORDER BY time DESC LIMIT %d ;",
        "add" : "INSERT INTO activity (time, name, amount) VALUES (%d, '%s', %d);",
    ]) ;
}

mixed new_account(string name) {
    string query ;
    mixed result ;

    // Check if the account already exists
    name = capitalize(lower_case(name)) ;
    result = query_balance(name) ;
// _debug("new_account: result: %O", result) ;
// _debug("new_account: typeof result: %O", typeof(result)) ;
// _debug("new_account: nullp(result): %O", nullp(result)) ;
    if(stringp(result))
        return result ;

    if(!nullp(result))
        return "Account already exists." ;

    // Create the account
    query = sprintf(balance_statements["add"], name, 0, time()) ;
    result = DB_D->query(db, query) ;

    // add activity
    query = sprintf(activity_statements["add"], time(), name, 0) ;
    result = DB_D->query(db, query) ;

    return 1 ;
}

mixed query_balance(string name) {
    string query ;
    mixed result ;

    name = capitalize(lower_case(name)) ;
    query = sprintf(balance_statements["select"], name) ;

    result = DB_D->query(db, query) ;
// _debug("query_balance: result: %O", result) ;
// _debug("query_balance: typeof result: %O", typeof(result)) ;
// _debug("nullp(result): %O", nullp(result)) ;
    if(stringp(result)) {
        // _debug("query_balance: stringp(result): %O", result) ;
        return result ;
    }

    if(sizeof(result) == 0) {
// _debug("query_balance: sizeof(result) == 0") ;
        return null ;
    }
// _debug("query_balance: result[0]: %O", result[0]) ;
    return result[0]["amount"] ;
}

mixed add_balance(string name, int amount) {
    string query ;
    mixed result ;

    name = capitalize(lower_case(name)) ;
    result = query_balance(name) ;

    if(stringp(result))
        return result ;

    if(nullp(result))
        return "Account does not exist." ;

    result += amount ;
    if(result < 0)
        return "Insufficient funds." ;

    query = sprintf(balance_statements["update"], result, time(), name) ;
    result = DB_D->query(db, query) ;

    // add activity
    query = sprintf(activity_statements["add"], time(), name, amount) ;
    result = DB_D->query(db, query) ;

    if(stringp(result))
        return result ;

    return 1 ;
}

varargs mixed query_activity(string name, int limit: (: 10 :)) {
    string query ;
    mixed result ;

    name = capitalize(lower_case(name)) ;
    if(limit > 0)
        query = sprintf(activity_statements["select_limited"], name, limit) ;
    else
        query = sprintf(activity_statements["select"], name) ;

    result = DB_D->query(db, query) ;

    if(stringp(result))
        return result ;

    if(sizeof(result) == 0)
        return null ;

    return result ;
}
