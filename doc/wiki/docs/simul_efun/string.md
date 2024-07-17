---
title: string
---
# string.c

## substr

### Synopsis

```c
varargs string substr(string str, string sub, int reverse)
```

### Parameters

* `string str` - The string to extract from.
* `string sub` - The substring to extract to.
* `int [reverse=0]` - If set, the substring will start at the last occurrence.

### Description

Returns a substring of a string, starting from 0 and ending at the
first occurrence of another string within it. If the reverse flag
is set, the substring will start at the last occurrence of the
substring within the string.

## extract

### Synopsis

```c
varargs string extract(string str, int from, int to)
```

### Parameters

* `string str` - The string to extract from.
* `int from` - The starting position to extract from.
* `int [to]` - The ending position to extract to.

### Description

Extracts a substring from a string.

## reverse_strsrch

### Synopsis

```c
varargs int reverse_strsrch(string str, string sub, int start)
```

### Parameters

* `string str` - The string to search in.
* `string sub` - The substring to search for.
* `int [start=-1]` - The starting position to search from.

### Description

Searches for a substring in a string starting from a given position
and moving backwards.

## append

### Synopsis

```c
string append(string source, string to_append)
```

### Parameters

* `string source` - The string to append to.
* `string to_append` - The string to append.

### Description

Appends a string to another string if it is not already there.
If the string is already present, the original string is returned.

## no_ansi

### Synopsis

```c
string no_ansi(string str)
```

### Parameters

* `string str` - The string to remove ANSI codes from.

### Description

Returns a string with all ANSI colour codes removed.

## chop

### Synopsis

```c
varargs string chop(string str, string sub, int dir)
```

### Parameters

* `string str` - The string to chop from.
* `string sub` - The substring to chop.
* `int [dir=-1]` - The direction to chop: 1 for left, -1 for right.

### Description

Chops a substring off the end or beginning of another string if
it is present. If the substring is not present, the original
string is returned.

## from_string

### Synopsis

```c
varargs mixed from_string(string str, int flag)
```

### Parameters

* `string str` - The string to convert.
* `int [flag=0]` - If set, returns an array with the value and the remaining string.

### Description

Converts a string representation of an LPC value to the corresponding
LPC value.

## reverse_string

### Synopsis

```c
string reverse_string(string str)
```

### Parameters

* `string str` - The string to reverse.

### Description

Reverses a string.

## simple_list

### Synopsis

```c
varargs string simple_list(string *arr, string conjunction)
```

### Parameters

* `string[] arr` - The array to make a list from.
* `string [conjunction="and"]` - The word to join the last two elements of the list.

### Description

Returns a string that is a simple list of the elements of an array,
joined by a conjunction.

## add_commas

### Synopsis

```c
string add_commas(mixed number)
```

### Parameters

* `mixed number` - The number to add commas to.

### Description

Returns a string with commas added to the number.

## stringify

### Synopsis

```c
string stringify(mixed val)
```

### Parameters

* `mixed val` - The value to convert.

### Description

Converts an LPC value to its string representation.

## prepend

### Synopsis

```c
string prepend(string source, string to_prepend)
```

### Parameters

* `string source` - The string to prepend to.
* `string to_prepend` - The string to prepend.

### Description

Prepends a string to another string if it is not already there.
If the string is already present, the original string is returned.

