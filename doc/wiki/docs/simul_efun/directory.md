---
title: directory
---
# directory.c

## query_directory

### Synopsis

```c
string query_directory(object ob)
```

### Parameters

* `object ob` - The object to query the directory of.

### Description

Returns the directory of the given object. If no object is
provided, it defaults to the previous object.

## assure_dir

### Synopsis

```c
mixed assure_dir(string path)
```

### Parameters

* `string path` - The path of the directory to ensure.

### Description

Ensures that a directory exists by creating it and its parent
directories if necessary.

