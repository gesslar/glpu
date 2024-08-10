/**
 * @file /adm/daemons/account.c
 * @description The account daemon is responsible for managing player accounts.
 *
 * @created 2024/08/09 - Gesslar
 * @last_modified 2024/08/09 - Gesslar
 *
 * @history
 * 2024/08/09 - Gesslar - Created
 */

inherit STD_DAEMON ;

// Functions
int create_account(string name, string password) ;
mapping load_account(string name) ;
string write_account(string name, string key, mixed data) ;
mixed read_account(string name, string key) ;
int valid_manip(string name) ;

void setup() {
    set_no_clean(1) ;
}

int create_account(string name, string password) {
    mapping account ;

    if(!valid_manip(name))
        return 0 ;

    if(!name || !stringp(name))
        return 0 ;

    if(!password || !stringp(password))
        return 0 ;

    if(valid_account(name))
        return 0 ;

    if(password[0..2] != "$6$")
        return 0 ;

    name = lower_case(name) ;

    account = ([ "name" : name, "password" : password ]) ;

    write_file(account_file(name), pretty_map(account)) ;

    return 1 ;
}

mapping load_account(string name) {
    string file ;

    if(!valid_manip(name))
        return 0 ;

    if(!name || !stringp(name))
        return 0 ;

    if(!valid_account(name))
        return 0 ;

    file = account_file(name) ;

    return from_string(read_file(file)) ;
}

string write_account(string name, string key, mixed data) {
    mapping account ;

    if(!valid_manip(name))
        return 0 ;

    if(!name || !stringp(name))
        return 0 ;

    if(!key || !stringp(key))
        return 0 ;

    if(!data)
        return 0 ;

    account = load_account(name) ;

    if(!account)
        return 0 ;

    account[key] = data ;

    write_file(account_file(name), pretty_map(account)) ;

    return account[key] ;
}

mixed read_account(string name, string key) {
    mapping account ;

    if(!valid_manip(name))
        return 0 ;

    if(!name || !stringp(name))
        return 0 ;

    if(!key || !stringp(key))
        return 0 ;

    account = load_account(name) ;

    if(!account)
        return 0 ;

    return account[key] ;
}

int valid_manip(string name) {
    object prev = previous_object() ;
    object caller = this_caller() ;

    if(!prev && !caller)
        return 0 ;

    if(!is_member(query_privs(prev), "admin") &&
       query_privs(prev) != name &&
       query_privs(caller) != name)
        return 0 ;

    return 1 ;
}
