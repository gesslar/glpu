---
title: resolve_path
---
# resolve_path.c

## resolve_file

### Synopsis

```c
string resolve_file(string base_dir, string path)
```

### Parameters

* `string base_dir` - The base directory to resolve relative paths from.
* `string path` - The file path to resolve.

### Returns

`string` - The resolved absolute file path.

### Description

Resolves a file path without checking its existence.

## valid_file

### Synopsis

```c
string valid_file(string base_dir, string path)
```

### Parameters

* `string base_dir` - The base directory to resolve relative paths from.
* `string path` - The file path to resolve and validate.

### Returns

`string|int` - The resolved absolute file path if valid, or 0 if invalid.

### Description

Resolves and validates a file path, checking if it exists as a
file.

## get_files

### Synopsis

```c
string *get_files(string base_dir, string path)
```

### Parameters

* `string base_dir` - The base directory to resolve relative paths from.
* `string path` - The path or pattern to resolve and search for files.

### Returns

`string*` - An array of matching file paths, or ({}) if invalid.

### Description

Resolves a path and returns an array of matching files, supporting * wildcard pattern.

## resolve_dir

### Synopsis

```c
string resolve_dir(string base_dir, string path)
```

### Parameters

* `string base_dir` - The base directory to resolve relative paths from.
* `string path` - The directory path to resolve.

### Returns

`string` - The resolved absolute directory path, ending with a slash.

### Description

Resolves a directory path without checking its existence,
ensuring it ends with a slash.

## valid_dir

### Synopsis

```c
string valid_dir(string base_dir, string path)
```

### Parameters

* `string base_dir` - The base directory to resolve relative paths from.
* `string path` - The directory path to resolve and validate.

### Returns

`string|int` - The resolved absolute directory path if valid, or 0 if invalid.

### Description

Resolves and validates a directory path, checking if it exists
as a directory.

## resolve_path

### Synopsis

```c
string resolve_path(string base_dir, string path)
```

### Parameters

* `string Current` - The current path.
* `string Next` - The next path to resolve.

### Returns

`string` - The resolved absolute path.

### Description

Resolves a given path relative to the current path, handling
special cases such as user directories and relative paths.

## valid_path

### Synopsis

```c
string valid_path(string base_dir, string path)
```

### Parameters

* `string base_dir` - The base directory to resolve relative paths from.
* `string path` - The path to resolve and validate.

### Returns

`string|int` - The resolved absolute path if valid, or 0 if invalid.

### Description

Resolves and validates a path, checking if it exists as either
a file or directory.

