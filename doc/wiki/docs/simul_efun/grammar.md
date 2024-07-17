---
layout: default
title: grammar
---
# grammar.c

## consolidate

### Synopsis

```c
string consolidate(int x, string str)
```

### Parameters

* `int x` - The quantity of items.
* `string str` - The description of the item(s).

### Description

Returns a consolidated string for a given quantity and item
description, handling pluralization and special cases.

## ordinal

### Synopsis

```c
string ordinal (int n)
```

### Parameters

* `int n` - The integer to convert.

### Description

Converts an integer to its ordinal string representation.

## int_string

### Synopsis

```c
string int_string (int num)
```

### Parameters

* `int num` - The numerical number to convert.

### Description

Converts a numerical number to its worded number representation.

