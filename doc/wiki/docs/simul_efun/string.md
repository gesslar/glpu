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

### Returns

`string` - The extracted substring.

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

### Returns

`string` - The extracted substring.

### Description

Extracts a substring from a string.

## pcre_strsrch

### Synopsis

```c
varargs int pcre_strsrch(string str, string substr, int reverse)
```

### Parameters

* `string str` - The string to search in.
* `string substr` - The regular expression to search for.
* `int [reverse=0]` - If set, the search will start from the end of the string.

### Returns

`int` - The position of the substring in the string, or -1 if not found.

### Description

Searches for the position of a substring in a string using a
regular expression.

## reverse_strsrch

### Synopsis

```c
varargs int reverse_strsrch(string str, string sub, int start)
```

### Parameters

* `string str` - The string to search in.
* `string sub` - The substring to search for.
* `int [start=-1]` - The starting position to search from.

### Returns

`int` - The position of the substring in the string, or -1 if not found.

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

### Returns

`string` - The original string with the appended string if it was not already present.

### Description

Appends a string to another string if it is not already there.
If the string is already present, the original string is returned.

## no_ansi

### Synopsis

```c
string no_ansi(string str)
```

### Parameters

* `string str` - The string to remove colour from.

### Returns

`string` - The string without colour.

### Description

Returns a string with all colour removed.

## chop

### Synopsis

```c
varargs string chop(string str, string sub, int dir)
```

### Parameters

* `string str` - The string to chop from.
* `string sub` - The substring to chop.
* `int [dir=-1]` - The direction to chop: 1 for left, -1 for right.

### Returns

`string` - The string with the substring chopped off if it was present.

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

### Returns

`mixed` - The LPC value represented by the string.

### Description

Converts a string representation of an LPC value to the
corresponding LPC value.

## reverse_string

### Synopsis

```c
string reverse_string(string str)
```

### Parameters

* `string str` - The string to reverse.

### Returns

`string` - The reversed string.

### Description

Reverses a string.

## simple_list

### Synopsis

```c
varargs string simple_list(string *arr, string conjunction)
```

### Parameters

* `string* arr` - The array to make a list from.
* `string [conjunction="and"]` - The word to join the last two elements of the list.

### Returns

`string` - The simple list string.

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

### Returns

`string` - The number with commas added as a string.

### Description

Returns a string with commas added to the number.

## stringify

### Synopsis

```c
string stringify(mixed val)
```

### Parameters

* `mixed val` - The value to convert.

### Returns

`string` - The string representation of the value.

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

### Returns

`string` - The original string with the prepended string if it was not already present.

### Description

Prepends a string to another string if it is not already there.
If the string is already present, the original string is returned.

