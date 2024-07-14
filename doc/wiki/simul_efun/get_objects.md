# get_objects

## SYNOPSIS

    varargs mixed get_objects( string str, object player, int no_arr )

### PARAMETERS

    string str - The search string specifying the objects to locate.
    object [player] - The player object to use as a reference for searching.
    int [no_arr] - If specified, only a single object or 0 will be returned, otherwise an array of objects may be returned.

### RETURNS

    mixed - - The located object(s), or 0 if not found.

## DESCRIPTION

Locates objects based on the specified search string, which can
include various search criteria and options.
