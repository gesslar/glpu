---
title: exists
---
# exists.c

## file_exists

### Synopsis

```c
int file_exists(string file)
```

### Parameters

* `string file` - The name of the file to check.

### Returns

`int` - 1 if the file exists, otherwise 0.

### Description

Checks if a file exists.

## user_exists

### Synopsis

```c
int user_exists(string user)
```

### Parameters

* `string user` - The username to check.

### Returns

`int` - 1 if the user data file exists, otherwise 0.

### Description

Checks if a user data file exists.

## cfile_exists

### Synopsis

```c
int cfile_exists(string file)
```

### Parameters

* `string file` - The base name of the file to check.

### Returns

`int` - 1 if the compiled file exists, otherwise 0.

### Description

Checks if a compiled file (.c) exists.

## ofile_exists

### Synopsis

```c
int ofile_exists(string file)
```

### Parameters

* `string file` - The base name of the file to check.

### Returns

`int` - 1 if the save file exists, otherwise 0.

### Description

Checks if a save file exists.

## directory_exists

### Synopsis

```c
int directory_exists(string dirname)
```

### Parameters

* `string dirname` - The name of the directory to check.

### Returns

`int` - 1 if the directory exists, otherwise 0.

### Description

Checks if a directory exists.

