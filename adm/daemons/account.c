/**
 * @file /adm/daemons/account.c
 *
 * Account daemon responsible for managing player accounts, including creation,
 * modification, and character associations.
 *
 * @created 2024-08-09 - Gesslar
 * @last_modified 2024-08-09 - Gesslar
 *
 * @history
 * 2024-08-09 - Gesslar - Created
 */

inherit STD_DAEMON;

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

private nomask mapping accounts = ([ ]);
private nomask mapping reverse = ([ ]);

/**
 * Configures the daemon to prevent automatic cleanup and sets persistence.
 */
void setup() {
  set_no_clean(1);
  set_persistent(1);
}

/**
 * Creates a new account with the given name and password.
 *
 * @param {string} name - The account name to create
 * @param {string} password - The encrypted password (must start with $6$)
 * @returns {int} 1 on success, 0 on failure
 */
int create_account(string name, string password) {
  mapping account;

  if(!valid_manip(name))
    return 0;

  if(!name || !stringp(name))
    return 0;

  if(!password || !stringp(password))
    return 0;

  if(valid_account(name))
    return 0;

  if(password[0..2] != "$6$")
    return 0;

  name = lower_case(name);

  account = ([
    "password" : password,
    "characters" : ({})
  ]);

  accounts[name] = account;

  save_data();

  return 1;
}

/**
 * Loads an account's data from storage.
 *
 * If the account is not in memory, it will be loaded from disk and cached.
 * The disk file will be removed after loading.
 *
 * @param {string} name - The account name to load
 * @returns {mapping} The account data if found, null otherwise
 */
mapping load_account(string name) {
  string file;

  if(!valid_manip(name))
    return null;

  if(!name || !stringp(name))
    return null;

  if(!accounts[name]) {
    file = account_file(name);

    if(!file_exists(file))
      return null;

    accounts[name] = from_string(read_file(file));
    reverse[file] = name;

    rm(file);
  }

  return accounts[name];
}

/**
 * Updates a specific key-value pair in an account.
 *
 * @param {string} name - The account name to modify
 * @param {string} key - The key to update
 * @param {mixed} data - The new value to store
 * @returns {mixed} The stored value on success, 0 on failure
 */
string write_account(string name, string key, mixed data) {
  mapping account;

  if(!valid_manip(name))
    return 0;

  if(!name || !stringp(name))
    return 0;

  if(!key || !stringp(key))
    return 0;

  if(!data)
    return 0;

  account = load_account(name);

  if(!account)
    return 0;

  account[key] = data;

  write_file(account_file(name), pretty_map(account));

  save_data();

  return account[key];
}

/**
 * Retrieves a specific value from an account.
 *
 * @param {string} name - The account name to read from
 * @param {string} key - The key to retrieve
 * @returns {mixed} The stored value if found, 0 otherwise
 */
mixed read_account(string name, string key) {
  mapping account;

  if(!valid_manip(name))
    return 0;

  if(!name || !stringp(name))
    return 0;

  if(!key || !stringp(key))
    return 0;

  account = load_account(name);

  if(!account)
    return 0;

  return account[key];
}

/**
 * Validates if the caller has permission to manipulate an account.
 *
 * Permissions are granted if:
 * - The caller has admin privileges
 * - The caller's privs match the account name
 * - The caller is the GMCP Char module
 *
 * @param {string} name - The account name to check permissions for
 * @returns {int} 1 if manipulation is allowed, 0 otherwise
 */
int valid_manip(string name) {
  object prev = previous_object();
  object caller = this_caller();

  if(!prev && !caller)
    return 0;

  if(!is_member(query_privs(prev), "admin") &&
     query_privs(prev) != name &&
     base_name(previous_object()) != "/std/modules/gmcp/Char" &&
     (caller && query_privs(caller)) != name
  )
    return 0;

  return 1;
}

/**
 * Removes an account and all its associated data.
 *
 * This removes both the account data and any character associations.
 *
 * @param {string} name - The account name to remove
 * @returns {int} 1 on success, 0 on failure
 */
int remove_account(string name) {
  if(!valid_manip(name))
    return 0;

  if(!name || !stringp(name))
    return 0;

  if(!valid_account(name))
    return 0;

  map_delete(accounts, name);
  foreach(string key, string value in reverse) {
    if(value == name)
      map_delete(reverse, key);
  }

  save_data();

  return 1;
}

/**
 * Associates a character with an account.
 *
 * @param {string} account_name - The account to add the character to
 * @param {string} str - The character name to add
 * @returns {int} 1 on success, null on failure
 */
int add_character(string account_name, string str) {
  mapping account;
  string *characters;

  if(!account_name || !stringp(account_name))
    return null;

  if(!valid_manip(account_name))
    return null;

  if(!accounts[account_name])
    return null;

  if(!str || !stringp(str))
    return null;

  str = lower_case(str);

  account = load_account(account_name);

  if(!account)
    return 0;

  characters = account["characters"] || ({});
  characters += ({ str });
  account["characters"] = distinct_array(characters);
  accounts[account_name] = account;
  reverse[str] = account_name;

  save_data();

  return 1;
}

/**
 * Removes a character from an account.
 *
 * @param {string} account_name - The account to remove the character from
 * @param {string} str - The character name to remove
 * @returns {int} 1 on success, null on failure
 */
int remove_character(string account_name, string str) {
  mapping account;
  string *characters;

  if(!account_name || !stringp(account_name))
    return null;

  if(!valid_manip(account_name))
    return null;

  if(!accounts[account_name])
    return null;

  if(!str || !stringp(str))
      return null;

  str = lower_case(str);

  account = load_account(account_name);

  if(!account)
    return 0;

  characters = account["characters"] || ({});
  characters -= ({ str });
  account["characters"] = characters;
  accounts[account_name] = account;

  map_delete(reverse, str);

  save_data();

  return 1;
}

/**
 * Retrieves the account name associated with a character.
 *
 * @param {string} str - The character name to look up
 * @returns {string} The associated account name if found, null otherwise
 */
string character_account(string str) {
  if(!str || !stringp(str))
    return null;

  if(!reverse[str])
    return null;

  return reverse[str];
}

/**
 * Retrieves all characters associated with an account.
 *
 * @param {string} account_name - The account name to look up
* @returns {string *} Array of character names if found, null otherwise
*/
string *account_characters(string account_name) {
  mapping account;

  if(!account_name || !stringp(account_name))
    return null;

  if(!valid_manip(account_name))
    return null;

  if(!accounts[account_name])
    return null;

  account = load_account(account_name);

  if(!account)
    return 0;

  return account["characters"] || ({});
}
