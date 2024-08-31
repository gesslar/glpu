---
title: save
---
# save.c

## object_data_directory

### Synopsis

```c
string object_data_directory(object ob)
```

### Parameters

* `object ob` - The object to get the data directory for.

### Returns

`string` - The data directory path for the object.

### Description

Returns the data directory path for the specified object.

## object_data_file

### Synopsis

```c
string object_data_file(object ob)
```

### Parameters

* `object ob` - The object to get the data file for.

### Returns

`string` - The data file path for the object.

### Description

Returns the data file path for the specified object.

## assure_object_data_dir

### Synopsis

```c
string assure_object_data_dir(object ob)
```

### Parameters

* `object ob` - The object to ensure the data directory for.

### Returns

`string` - The data directory path for the object, or 0 if creation failed.

### Description

Ensures the data directory for the specified object exists,
creating it if necessary.

