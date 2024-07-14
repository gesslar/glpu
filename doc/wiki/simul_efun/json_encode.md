# json_encode

## SYNOPSIS

    varargs string json_encode(mixed value, mixed* pointers)

### PARAMETERS

    mixed value - The LPC value to serialize.
    mixed[] [pointers] - An optional array of pointers to handle circular references.

### RETURNS

    string - - The JSON string representation of the LPC value.

## DESCRIPTION

Serializes an LPC value into a JSON string.
