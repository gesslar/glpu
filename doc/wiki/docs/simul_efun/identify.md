---
title: identify
---
# identify.c

## identify

### Synopsis

```c
varargs string identify( mixed a, string indent )
```

### Parameters

* `mixed a` - The variable to identify.
* `string [indent]` - The indentation string to use for formatting.

### Returns

`string` - The string representation of the variable.

### Description

Converts a variable to its string representation for debugging
purposes, handling various data types and preventing deep
recursion.

