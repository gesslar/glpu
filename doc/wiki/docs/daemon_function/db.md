---
title: db
---
# db.c

## statement_from_mapping

### Synopsis

```c
string statement_from_mapping(mapping data)
```

### Parameters

* `mapping data` - The mapping containing column names and values.

### Returns

`string` - The generated SQL statement or null if the mapping is empty.

### Description

Generates a SQL statement from a mapping of column-value pairs.

## valid_table

### Synopsis

```c
int valid_table(string db, string table)
```

### Parameters

* `string db` - The name of the database.
* `string table` - The name of the table to check.

### Returns

`int` - 1 if the table exists, 0 otherwise.

### Description

Checks if a given table exists in a specified database.

## valid_db

### Synopsis

```c
int valid_db(string db)
```

### Parameters

* `string db` - The name of the database to check.

### Returns

`int` - 1 if the database is valid, 0 otherwise.

### Description

Checks if a given database is valid and exists in the system.

## query_databases

### Synopsis

```c
mapping query_databases()
```

### Returns

`mapping` - A mapping of database names to their file paths.

### Description

Retrieves a list of all available databases.

## lazy_query

### Synopsis

```c
void lazy_query(string db, string q, mixed *callback)
```

### Parameters

* `string db` - The name of the database to query.
* `string q` - The SQL query to execute.
* `mixed* callback` - Callback function to handle the result.

### Description

Initiates a lazy (chunked) query execution.

## sqlite_version

### Synopsis

```c
mixed sqlite_version(string db)
```

### Parameters

* `string db` - The name of the database.

### Returns

`mixed` - An array of version numbers or an error code.

### Description

Retrieves the SQLite version for the specified database.

## query_tables

### Synopsis

```c
mapping query_tables(string db_name)
```

### Parameters

* `string db_name` - The name of the database.

### Returns

`mapping` - A mapping of table names to their definitions.

### Description

Retrieves a list of tables for a specified database.

## query

### Synopsis

```c
mixed query(string db, string q, mixed *callback)
```

### Parameters

* `string db` - The name of the database to query.
* `string q` - The SQL query to execute.
* `mixed* callback` - Optional callback function to handle the result.

### Returns

`mixed` - Query result or 1 if callback is provided.

### Description

Executes a SQL query on the specified database.

## allow_upsert

### Synopsis

```c
int allow_upsert(string db)
```

### Parameters

* `string db` - The name of the database to check.

### Returns

`int` - 1 if upsert is supported, 0 otherwise.

### Description

Checks if the SQLite version supports upsert operations.

