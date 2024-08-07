---
title: save
---
# save.c

## object_save_directory

### Synopsis

```c
string object_save_directory(object ob)
```

### Parameters

* `object ob` - The object to get the save directory for.

### Returns

`string` - The save directory path for the object.

### Description

Returns the save directory path for the specified object.

## assure_object_save_dir

### Synopsis

```c
string assure_object_save_dir(object ob)
```

### Parameters

* `object ob` - The object to ensure the save directory for.

### Returns

`string` - The save directory path for the object, or 0 if creation failed.

### Description

Ensures the save directory for the specified object exists,
creating it if necessary.

## object_save_file

### Synopsis

```c
string object_save_file(object ob)
```

### Parameters

* `object ob` - The object to get the save file for.

### Returns

`string` - The save file path for the object.

### Description

Returns the save file path for the specified object.

