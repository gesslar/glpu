# remove_array_element

## SYNOPSIS

    varargs mixed *remove_array_element(mixed *arr, int start, int end)

### PARAMETERS

    mixed[] arr - The input array.
    int start - The starting index of elements to be removed.
    int [end] - The ending index of elements to be removed. Defaults to start if not specified.

### RETURNS

    mixed[] - - A new array with specified elements removed.

## DESCRIPTION

Returns a new array containing the elements of the input array
from index 0 to start-1, and from end+1 to the end of the input
array. If start is greater than end, the new array will contain
all the elements of the input array.
