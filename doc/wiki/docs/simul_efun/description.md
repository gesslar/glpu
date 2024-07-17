---
title: description
---
# description.c

## get_long

### Synopsis

```c
string get_long(object ob, int extras: (: 1 :))
```

### Parameters

* `object ob` - The object to get the long description of.
* `int [extras=1]` - Whether to include extra long descriptions. Defaults to 1 (include extras).

### Description

Returns the long description of an object, optionally
including extra long descriptions.

## get_short

### Synopsis

```c
string get_short(object ob, int extras: (: 1 :))
```

### Parameters

* `object ob` - The object to get the short description of.
* `int [extras=1]` - Whether to include extra short descriptions. Defaults to 1 (include extras).

### Description

Returns the short description of an object, optionally
including extra short descriptions in parentheses.

