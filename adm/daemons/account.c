/**
 * @file /adm/daemons/account.c
 * @description The account daemon is responsible for managing player accounts.
 *
 * @created 2024-08-09 - Gesslar
 * @last_modified 2024-08-09 - Gesslar
 *
 * @history
 * 2024-08-09 - Gesslar - Created
 */

inherit STD_DAEMON ;

// Forward declarations
int create_account(string name, string password);
mapping load_account(string name);
string write_account(string name, string key, mixed data);
mixed read_account(string name, string key);
int valid_manip(string name);
int remove_account(string name);
int add_character(string account_name, string str);
int remove_character(string account_name, string str);
string character_account(string str);
string *account_characters(string account_name);

private nomask mapping accounts = ([ ]) ;
private nomask mapping reverse = ([ ]) ;

void setup() {
    set_no_clean(1) ;
    set_persistent(1) ;
}

/**
 * @daemon_function create_account
 * @description Creates a new account with the given name and password.
 * @param {string} name - The name for the new account.
 * @param {string} password - The password for the new account.
 * @returns {int} 1 if the account was created successfully, 0 otherwise.
 */
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

/**
 * @daemon_function load_account
 * @description Loads an account from the database or memory.
 * @param {string} name - The name of the account to load.
 * @returns {mapping} The account data if found, null otherwise.
 */
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

/**
 * @daemon_function write_account
 * @description Writes a specific key-value pair to an account.
 * @param {string} name - The name of the account to modify.
 * @param {string} key - The key to write.
 * @param {mixed} data - The data to write.
 * @returns {string} The written data if successful, 0 otherwise.
 */
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

/**
 * @daemon_function read_account
 * @description Reads a specific key from an account.
 * @param {string} name - The name of the account to read from.
 * @param {string} key - The key to read.
 * @returns {mixed} The value of the key if found, 0 otherwise.
 */
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

/**
 * @daemon_function valid_manip
 * @description Checks if the current object has permission to manipulate the account.
 * @param {string} name - The name of the account to check.
 * @returns {int} 1 if manipulation is allowed, 0 otherwise.
 */
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

/**
 * @daemon_function remove_account
 * @description Removes an account from the database.
 * @param {string} name - The name of the account to remove.
 * @returns {int} 1 if the account was removed successfully, 0 otherwise.
 */
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

/**
 * @daemon_function add_character
 * @description Adds a character to an account.
 * @param {string} account_name - The name of the account to add the character to.
 * @param {string} str - The name of the character to add.
 * @returns {int} 1 if the character was added successfully, null otherwise.
 */
int add_character(string account_name, string str) {
    mapping account ;
    string *characters ;

    if(!account_name || !stringp(account_name))
        return null ;

    if(!valid_manip(account_name))
        return null ;

    if(!accounts[account_name])
        return null ;

    if(!str || !stringp(str))
        return null ;

    str = lower_case(str) ;

    // if(user_exists(str))
    //     return 0 ;

    account = load_account(account_name) ;

    if(!account)
        return 0 ;

    characters = account["characters"] || ({}) ;
    characters += ({ str }) ;
    account["characters"] = distinct_array(characters) ;
    accounts[account_name] = account ;
    reverse[str] = account_name ;

    save_data() ;

    return 1 ;
}

/**
 * @daemon_function remove_character
 * @description Removes a character from an account.
 * @param {string} account_name - The name of the account to remove the character from.
 * @param {string} str - The name of the character to remove.
 * @returns {int} 1 if the character was removed successfully, null otherwise.
 */
int remove_character(string account_name, string str) {
    mapping account ;
    string *characters ;

    if(!account_name || !stringp(account_name))
        return null ;

    if(!valid_manip(account_name))
        return null ;

    if(!accounts[account_name])
        return null ;

    if(!str || !stringp(str))
        return null ;

    str = lower_case(str) ;

    account = load_account(account_name) ;

    if(!account)
        return 0 ;

    characters = account["characters"] || ({}) ;
    characters -= ({ str }) ;
    account["characters"] = characters ;
    accounts[account_name] = account ;
    map_delete(reverse, str) ;

    save_data() ;

    return 1 ;
}

/**
 * @daemon_function character_account
 * @description Retrieves the account name associated with a character.
 * @param {string} str - The name of the character.
 * @returns {string} The account name if found, null otherwise.
 */
string character_account(string str) {
    if(!str || !stringp(str))
        return null ;

    if(!reverse[str])
        return null ;

    return reverse[str] ;
}

/**
 * @daemon_function account_characters
 * @description Retrieves the characters associated with an account.
 * @param {string} account_name - The name of the account.
 * @returns {string *} An array of character names if found, null otherwise.
 */
string *account_characters(string account_name) {
    mapping account ;

    if(!account_name || !stringp(account_name))
        return null ;

    if(!valid_manip(account_name))
        return null ;

    if(!accounts[account_name])
        return null ;

    account = load_account(account_name) ;

    if(!account)
        return 0 ;

    return account["characters"] || ({}) ;
}
