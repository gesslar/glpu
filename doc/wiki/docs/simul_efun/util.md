---
title: util
---
# util.c

## generate_uuid

### Synopsis

```c
string generate_uuid()
```

### Returns

`string` - A randomly generated version 4 UUID.

### Description

Generates a random "version 4" UUID. Version 4 UUIDs are based
on random numbers.

## of

### Synopsis

```c
int of(mixed needle, mixed haystack)
```

### Parameters

* `mixed needle` - The value to search for.
* `mixed haystack ` - The array, mapping, or string to search in.

### Returns

`int` - 1 if the value is found, 0 otherwise.

### Description

Checks if a value is present in an array, mapping, or string.

