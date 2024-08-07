---
title: description
---
# description.c

## get_long

### Synopsis

```c
string get_long(object ob, int extras)
```

### Parameters

* `object ob` - The object to get the long description of.
* `int [extras=1]` - Whether to include extra long descriptions. Defaults to 1 (include extras).

### Returns

`string` - The long description of the object, including any extra long descriptions.

### Description

Returns the long description of an object, optionally
including extra long descriptions.

## get_short

### Synopsis

```c
varargs string get_short(object ob, int extras)
```

### Parameters

* `object ob` - The object to get the short description of.
* `int [extras=1]` - Whether to include extra short descriptions. Defaults to 1 (include extras).

### Returns

`string` - The short description of the object, including any extra short descriptions.

### Description

Returns the short description of an object, optionally
including extra short descriptions in parentheses.

