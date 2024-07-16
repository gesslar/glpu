## substr

## SYNOPSIS

    varargs string substr(string str, string sub, int reverse)

### PARAMETERS

    string str - The string to extract from.
    string sub - The substring to extract to.
    int [reverse=0] - If set, the substring will start at the last occurrence.

### RETURNS

    string - The extracted substring.

## DESCRIPTION

Returns a substring of a string, starting from 0 and ending at the
first occurrence of another string within it. If the reverse flag
is set, the substring will start at the last occurrence of the
substring within the string.

## extract

## SYNOPSIS

    varargs string extract(string str, int from, int to)

### PARAMETERS

    string str - The string to extract from.
    int from - The starting position to extract from.
    int [to] - The ending position to extract to.

### RETURNS

    string - The extracted substring.

## DESCRIPTION

Extracts a substring from a string.

## reverse_strsrch

## SYNOPSIS

    varargs int reverse_strsrch(string str, string sub, int start)

### PARAMETERS

    string str - The string to search in.
    string sub - The substring to search for.
    int [start=-1] - The starting position to search from.

### RETURNS

    int - The position of the substring in the string, or -1 if not found.

## DESCRIPTION

Searches for a substring in a string starting from a given position
and moving backwards.

## append

## SYNOPSIS

    string append(string source, string to_append)

### PARAMETERS

    string source - The string to append to.
    string to_append - The string to append.

### RETURNS

    string - The original string with the appended string if it was not already present.

## DESCRIPTION

Appends a string to another string if it is not already there.
If the string is already present, the original string is returned.

## no_ansi

## SYNOPSIS

    string no_ansi(string str)

### PARAMETERS

    string str - The string to remove ANSI codes from.

### RETURNS

    string - The string without ANSI codes.

## DESCRIPTION

Returns a string with all ANSI colour codes removed.

## chop

## SYNOPSIS

    varargs string chop(string str, string sub, int dir)

### PARAMETERS

    string str - The string to chop from.
    string sub - The substring to chop.
    int [dir=-1] - The direction to chop: 1 for left, -1 for right.

### RETURNS

    string - The string with the substring chopped off if it was present.

## DESCRIPTION

Chops a substring off the end or beginning of another string if
it is present. If the substring is not present, the original
string is returned.

## from_string

## SYNOPSIS

    varargs mixed from_string(string str, int flag)

### PARAMETERS

    string str - The string to convert.
    int [flag=0] - If set, returns an array with the value and the remaining string.

### RETURNS

    mixed - The LPC value represented by the string.

## DESCRIPTION

Converts a string representation of an LPC value to the corresponding
LPC value.

## reverse_string

## SYNOPSIS

    string reverse_string(string str)

### PARAMETERS

    string str - The string to reverse.

### RETURNS

    string - The reversed string.

## DESCRIPTION

Reverses a string.

## simple_list

## SYNOPSIS

    varargs string simple_list(string *arr, string conjunction)

### PARAMETERS

    string[] arr - The array to make a list from.
    string [conjunction="and"] - The word to join the last two elements of the list.

### RETURNS

    string - The simple list string.

## DESCRIPTION

Returns a string that is a simple list of the elements of an array,
joined by a conjunction.

## add_commas

## SYNOPSIS

    string add_commas(mixed number)

### PARAMETERS

    mixed number - The number to add commas to.

### RETURNS

    string - The number with commas added as a string.

## DESCRIPTION

Returns a string with commas added to the number.

## stringify

## SYNOPSIS

    string stringify(mixed val)

### PARAMETERS

    mixed val - The value to convert.

### RETURNS

    string - The string representation of the value.

## DESCRIPTION

Converts an LPC value to its string representation.

## prepend

## SYNOPSIS

    string prepend(string source, string to_prepend)

### PARAMETERS

    string source - The string to prepend to.
    string to_prepend - The string to prepend.

### RETURNS

    string - The original string with the prepended string if it was not already present.

## DESCRIPTION

Prepends a string to another string if it is not already there.
If the string is already present, the original string is returned.
