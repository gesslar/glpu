## data_inc

## SYNOPSIS

    varargs int data_inc(string file, string key, int inc)

### PARAMETERS

    string file - The file to modify.
    string key - The key to increment the value for.
    int inc - The amount to increment by.

### RETURNS

    int - The new value after incrementing.

## DESCRIPTION

Increments the value associated with the given key in the file
by the specified amount. If the key does not exist, it is
created with the increment value.

## data_del

## SYNOPSIS

    int data_del(string file, string key)

### PARAMETERS

    string file - The file to modify.
    string key - The key to delete.

### RETURNS

    int - 1 if the key was found and deleted, 0 otherwise.

## DESCRIPTION

Deletes the key-value pair from the file.

## data_value

## SYNOPSIS

    varargs mixed data_value(string file, string key, mixed def)

### PARAMETERS

    string file - The file to read from.
    string key - The key to search for.
    mixed [def] - The default value to return if the key is not found.

### RETURNS

    mixed - The value associated with the key, or the default value if the key is not found.

## DESCRIPTION

Retrieves the value associated with a given key from a file.

## data_write

## SYNOPSIS

    varargs void data_write(string file, string key, mixed data...)

### PARAMETERS

    string file - The file to write to.
    string key - The key to write.
    mixed data - The value(s) to write.

## DESCRIPTION

Writes a key-value pair to a file. If the key already exists,
the value is updated.

