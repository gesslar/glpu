---
title: bank
---
# bank.c

## query_activity

### Synopsis

```c
varargs mixed query_activity(string name, int limit: (: 10 :))
```

### Parameters

* `string name` - The name of the account holder.
* `int [limit=10]` - The maximum number of recent activities to retrieve.

### Returns

`mixed` - An array of recent activities if successful, null if there are no activities, or an error message if there was a database error.

### Description

Retrieves the recent activity for the given account.

## query_balance

### Synopsis

```c
mixed query_balance(string name)
```

### Parameters

* `string name` - The name of the account holder.

### Returns

`mixed` - The current balance as an integer if the account exists, null if the account doesn't exist, or an error message if there was a database error.

### Description

Retrieves the current balance for the given account name.

## add_balance

### Synopsis

```c
mixed add_balance(string name, int amount)
```

### Parameters

* `string name` - The name of the account holder.
* `int amount` - The amount to add (positive) or subtract (negative).

### Returns

`mixed` - A success message if the transaction was completed, an error message if the account doesn't exist, there are insufficient funds, or if there was a database error.

### Description

Adds (or subtracts) the specified amount from the given account.

## new_account

### Synopsis

```c
mixed new_account(string name)
```

### Parameters

* `string name` - The name of the account holder.

### Returns

`mixed` - 1 if the account was created successfully, an error message if the account already exists or if there was a database error.

### Description

Creates a new bank account for the given name.

