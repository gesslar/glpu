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

private nomask mapping accounts = ([ ]) ;
private nomask mapping reverse = ([ ]) ;

void setup() {
    set_no_clean(1) ;
    set_persistent(1) ;
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

    account = ([
        "password" : password,
        "characters" : ({})
    ]) ;

    accounts[name] = account ;

    save_data() ;

    return 1 ;
}

mapping load_account(string name) {
    string file ;

    if(!valid_manip(name))
        return null ;

    if(!name || !stringp(name))
        return null ;

    if(!accounts[name]) {
        file = account_file(name) ;

        if(!file_exists(file))
            return null ;

        accounts[name] = from_string(read_file(file)) ;
        reverse[file] = name ;

        rm(file) ;
    }

    return accounts[name] ;
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

    save_data() ;

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
       base_name(previous_object()) != "/std/modules/gmcp/Char" &&
       (caller && query_privs(caller)) != name)
        return 0 ;

    return 1 ;
}

int remove_account(string name) {
    if(!valid_manip(name))
        return 0 ;

    if(!name || !stringp(name))
        return 0 ;

    if(!valid_account(name))
        return 0 ;

    map_delete(accounts, name) ;
    foreach(string key, string value in reverse) {
        if(value == name)
            map_delete(reverse, key) ;
    }

    save_data() ;

    return 1 ;
}

int add_character(string account_name, string str) {
    mapping account ;
    string *characters ;

_debug("add_character: account_name: " + account_name + ", str: " + str) ;
    if(!account_name || !stringp(account_name))
        return null ;
_debug("have account_name") ;
    if(!valid_manip(account_name))
        return null ;
_debug("valid_manip") ;
_debug("account_name: %s", account_name) ;
_debug("accounts[%s]: %O", account_name, accounts[account_name]) ;

    if(!accounts[account_name])
        return null ;
_debug("valid_account") ;
    if(!str || !stringp(str))
        return null ;
_debug("have character name") ;
    str = lower_case(str) ;

    // if(user_exists(str))
    //     return 0 ;

    account = load_account(account_name) ;

    if(!account)
        return 0 ;
_debug("load_account") ;
    characters = account["characters"] || ({}) ;
    characters += ({ str }) ;
    account["characters"] = distinct_array(characters) ;
    accounts[account_name] = account ;
    reverse[str] = account_name ;

    save_data() ;

    return 1 ;
}
