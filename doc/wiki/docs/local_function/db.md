---
title: db
---
# db.c

## execute_query

### Synopsis

```c
private void execute_query(string db, string q, int offset, string query_id, mixed *callback)
```

### Parameters

* `string db` - The name of the database to query.
* `string q` - The SQL query to execute.
* `int offset` - The current offset for chunked queries.
* `string query_id` - A unique identifier for this query execution.
* `mixed* callback` - Callback function to handle the result.

### Description

Executes a query in chunks, handling large result sets efficiently.

## collate_data

### Synopsis

```c
private mapping *collate_data(mixed *result)
```

### Parameters

* `mixed* result` - The raw query result.

### Returns

`mapping*` - An array of mappings representing the collated data.

### Description

Collates query result data into a more usable format.

