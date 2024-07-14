# get_object

## SYNOPSIS

    varargs object get_object( string str, object player )

### PARAMETERS

    string str - The name of the object to locate.
    object [player] - The player object to use as a reference for searching.

### RETURNS

    object - - The located object, or 0 if not found.

## DESCRIPTION

Attempts to locate an object by the given name and returns the
object pointer if found.
