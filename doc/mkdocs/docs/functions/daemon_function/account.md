---
title: account
---
# account.c

## character_account

### Synopsis

```c
string character_account(string str)
```

### Parameters

* `string str` - The name of the character.

### Returns

{string} The account name if found, null otherwise.

### Description

Retrieves the account name associated with a character.

## create_account

### Synopsis

```c
int create_account(string name, string password)
```

### Parameters

* `string name` - The name for the new account.
* `string password` - The password for the new account.

### Returns

{int} 1 if the account was created successfully, 0 otherwise.

### Description

Creates a new account with the given name and password.

## remove_account

### Synopsis

```c
int remove_account(string name)
```

### Parameters

* `string name` - The name of the account to remove.

### Returns

{int} 1 if the account was removed successfully, 0 otherwise.

### Description

Removes an account from the database.

## add_character

### Synopsis

```c
int add_character(string account_name, string str)
```

### Parameters

* `string account_name` - The name of the account to add the character to.
* `string str` - The name of the character to add.

### Returns

{int} 1 if the character was added successfully, null otherwise.

### Description

Adds a character to an account.

## read_account

### Synopsis

```c
mixed read_account(string name, string key)
```

### Parameters

* `string name` - The name of the account to read from.
* `string key` - The key to read.

### Returns

{mixed} The value of the key if found, 0 otherwise.

### Description

Reads a specific key from an account.

## account_characters

### Synopsis

```c
string *account_characters(string account_name)
```

### Parameters

* `string account_name` - The name of the account.

### Returns

{string *} An array of character names if found, null otherwise.

### Description

Retrieves the characters associated with an account.

## load_account

### Synopsis

```c
mapping load_account(string name)
```

### Parameters

* `string name` - The name of the account to load.

### Returns

{mapping} The account data if found, null otherwise.

### Description

Loads an account from the database or memory.

## write_account

### Synopsis

```c
string write_account(string name, string key, mixed data)
```

### Parameters

* `string name` - The name of the account to modify.
* `string key` - The key to write.
* `mixed data` - The data to write.

### Returns

{string} The written data if successful, 0 otherwise.

### Description

Writes a specific key-value pair to an account.

## remove_character

### Synopsis

```c
int remove_character(string account_name, string str)
```

### Parameters

* `string account_name` - The name of the account to remove the character from.
* `string str` - The name of the character to remove.

### Returns

{int} 1 if the character was removed successfully, null otherwise.

### Description

Removes a character from an account.

## valid_manip

### Synopsis

```c
int valid_manip(string name)
```

### Parameters

* `string name` - The name of the account to check.

### Returns

{int} 1 if manipulation is allowed, 0 otherwise.

### Description

Checks if the current object has permission to manipulate the account.

