---
title: user
---
# user.c

## user_core_data_directory

### Synopsis

```c
string user_core_data_directory(string name)
```

### Parameters

* `string name` - The user's name.

### Returns

`string` - The directory path for the user's core data directory, or 0 if the input is invalid.

### Description

Returns the directory path for the user's core data directory based
on their name.

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

