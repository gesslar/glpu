---
title: arrays
---
# arrays.c

## reverse_array

### Synopsis

```c
mixed *reverse_array(mixed *arr)
```

### Parameters

* `mixed* arr` - The input array.

### Returns

`mixed*` - A new array with elements in reverse order.

### Description

Returns a new array with the elements of the input array in
reverse order.

## remove_array_element

### Synopsis

```c
varargs mixed *remove_array_element(mixed *arr, int start, int end)
```

### Parameters

* `mixed* arr` - The input array.
* `int start` - The starting index of elements to be removed.
* `int end` - The ending index of elements to be removed. Defaults to start if not specified.

### Returns

`mixed*` - A new array with specified elements removed.

### Description

Returns a new array containing the elements of the input array
from index 0 to start-1, and from end+1 to the end of the input
array. If start is greater than end, the new array will contain
all the elements of the input array.

## distinct_array

### Synopsis

```c
mixed *distinct_array(mixed *arr)
```

### Parameters

* `mixed* arr` - An array of mixed types.

### Returns

`mixed*` - A new array with distinct elements from the input array.

### Description

Returns a new array containing the distinct elements of the input
array.

## splice

### Synopsis

```c
varargs mixed *splice(mixed *arr, int start, int delete_count, mixed *items_to_add)
```

### Parameters

* `mixed* arr` - The array from which elements will be removed and to which new elements may be added.
* `int start` - The zero-based index at which to start changing the array. If negative, it will begin that many elements from the end.
* `int delete_count` - The number of elements to remove from the array, starting from the index specified by start. If delete_count is 0, no elements are removed.
* `mixed* [items_to_add]` - An array of elements to add to the array at the start index. Can be omitted or passed as null if no elements are to be added.

### Returns

`mixed*` - A new array reflecting the desired modifications.

### Description

Modifies the content of an array by removing existing elements
and/or adding new elements. Returns a new array with the
modifications.

