---
title: user
---
# user.c

## ghostp

### Synopsis

```c
int ghostp(object ob)
```

### Parameters

* `object ob` - The object to check.

### Returns

`int` - 1 if the object is a ghost, otherwise 0.

### Description

Returns 1 if the object is a ghost, 0 if not.

## account_path

### Synopsis

```c
string account_path(string name)
```

### Parameters

* `string name` - The name of the account.

### Returns

`string` - The path for the account file, or 0 if the input is invalid.

### Description

Returns the path for the account file.

## valid_account

### Synopsis

```c
int valid_account(string name)
```

### Parameters

* `string name` - The name of the account.

### Returns

`int` - 1 if the account is valid, otherwise 0.

### Description

Returns 1 if the account is valid, 0 if not.

## account_file

### Synopsis

```c
string account_file(string name)
```

### Parameters

* `string name` - The name of the account.

### Returns

`string` - The file path for the account file, or 0 if the input is invalid.

### Description

Returns the file path for the account file.

## user_path

### Synopsis

```c
string user_path(mixed name)
```

### Parameters

* `string|object name` - The user's name, or the body of the user.

### Returns

`string` - The home directory path for the user.

### Description

Returns the home directory path for the user.

## valid_user

### Synopsis

```c
varargs int valid_user(mixed ob)
```

### Parameters

* `mixed ob` - The object to check.

### Returns

`int` - 1 if the object is a valid user, otherwise 0.

### Description

Returns 1 if the object is a valid user, 0 if not.

## linkp

### Synopsis

```c
int linkp(object ob)
```

### Parameters

* `object ob` - The object to check.

### Returns

`int` - 1 if the object is a user object, otherwise 0.

### Description

Returns 1 if the object is a user object, 0 if not.

## user_inventory_data

### Synopsis

```c
string user_inventory_data(string name)
```

### Parameters

* `string name` - The user's name.

### Returns

`string` - The file path for the user's inventory data file, or 0 if the input is invalid.

### Description

Returns the file path for the user's inventory data file.

## user_data_directory

### Synopsis

```c
string user_data_directory(string name)
```

### Parameters

* `string name` - The user's name.

### Returns

`string` - The directory path for the user's data directory, or 0 if the input is invalid.

### Description

Returns the directory path for the user's data directory based on
their name.

## user_data_file

### Synopsis

```c
string user_data_file(string name)
```

### Parameters

* `string name` - The user's name.

### Returns

`string` - The file path for the user's data file, or 0 if the input is invalid.

### Description

Returns the file path for the user's data file.

## user_body_data

### Synopsis

```c
string user_body_data(string name)
```

### Parameters

* `string name` - The user's name.

### Returns

`string` - The file path for the user's mob data file, or 0 if the input is invalid.

### Description

Returns the file path for the user's mob data file.

