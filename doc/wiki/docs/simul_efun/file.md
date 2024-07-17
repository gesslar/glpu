---
layout: default
title: file
---
# file.c

## file_owner

### Synopsis

```c
string file_owner(string file)
```

### Parameters

* `string file` - The path of the file to check.

### Description

Determines the owner of a file based on its path.

## dir_file

### Synopsis

```c
string *dir_file(string path)
```

### Parameters

* `string path` - The path to extract the components from.

### Description

Given a path, returns an array containing the directory and file
name components.

## temp_file

### Synopsis

```c
varargs string temp_file(mixed arg)
```

### Parameters

* `mixed arg` - The file or object to create a temporary file for.

### Description

Generates a temporary file name based on the provided argument.

## assure_file

### Synopsis

```c
mixed assure_file(string file)
```

### Parameters

* `string file` - The path of the file to ensure.

### Description

Given a file, ensures that the directory structure leading to
the file exists, creating directories as needed.

## tail

### Synopsis

```c
varargs string tail(string path, int line_count)
```

### Parameters

* `string path` - The path of the file to read.
* `int [line_count=25]` - The number of lines to read from the end of the file. Defaults to 25.

### Description

Returns the last few lines of a file, similar to the Unix
`tail` command.

## implode_file

### Synopsis

```c
varargs void implode_file(string file, string *lines, int overwrite)
```

### Parameters

* `string file` - The path of the file to write to.
* `string[] lines` - The array of lines to write.
* `int [overwrite=0]` - Whether to overwrite the existing content. Defaults to 0 (append).

### Description

Writes an array of lines to a specified file, optionally
overwriting the existing content.

## log_file

### Synopsis

```c
varargs int log_file(string file, string str, mixed arg...)
```

### Parameters

* `string file` - The name of the log file.
* `string str` - The log message to write.
* `mixed [arg]` - Additional arguments to include in the log message.

### Description

Writes a log message to a specified log file.

## explode_file

### Synopsis

```c
string *explode_file(string file)
```

### Parameters

* `string file` - The path of the file to read.

### Description

Reads a file and returns its content as an array of lines,
excluding comment lines and empty lines.

## query_file_name

### Synopsis

```c
string query_file_name(object ob)
```

### Parameters

* `object ob` - The object to query the file name of.

### Description

Returns the name of the file corresponding to a given object.

