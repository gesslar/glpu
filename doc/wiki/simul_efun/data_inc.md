# data_inc

## SYNOPSIS

    varargs int data_inc(string file, string key, int inc)

### PARAMETERS

    string file - The file to modify.
    string key - The key to increment the value for.
    int inc - The amount to increment by.

### RETURNS

    int - - The new value after incrementing.

## DESCRIPTION

Increments the value associated with the given key in the file
by the specified amount. If the key does not exist, it is
created with the increment value.
