---
title: proc
---
# proc.c

## set_procs

### Synopsis

```c
void set_procs(mixed *procs)
```

### Parameters

* `mixed* procs` - The procs to set.

### Description

Set the procs for an object. You must pass the procs as an
2D array of names and mappings, strings, or function pointers.
* The first element of the array is the name of the proc.
* The second element of the array is the proc as a mapping,
string, or function pointer that follows the same criteria as
the individual elements of the arguments passed to add_proc.

## add_proc

### Synopsis

```c
void add_proc(string name, mixed proc)
```

### Parameters

* `string name` - The name of the proc to add.
* `mixed proc` - The proc to add as a mapping, string, or function pointer.

### Description

Add a proc to an object. You must pass the name of the proc and
the proc as a mapping, a string, or a valid function pointer.
* If you pass a string, it must be a function name in
this_object() that is either public or protected.
* If you pass a function pointer, it must be a valid function.
* If you pass a mapping, it must contain a function property.
The value of this property can be a string, a function
with the same criteria as above (string or function). When
cooldown and weight are not specified, the default values
are used.
* In the case of a string or a function pointer, the cooldown
and weight are set to the default values.

