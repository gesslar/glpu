# substr

## SYNOPSIS

    varargs string substr(string str, string sub, int reverse)

### PARAMETERS

    string str - The string to extract from.
    string sub - The substring to extract to.
    int [reverse=0] - If set, the substring will start at the last occurrence.

### RETURNS

    string - - The extracted substring.

## DESCRIPTION

Returns a substring of a string, starting from 0 and ending at the
first occurrence of another string within it. If the reverse flag
is set, the substring will start at the last occurrence of the
substring within the string.
