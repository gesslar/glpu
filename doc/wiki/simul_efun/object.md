## get_objects

## SYNOPSIS

    varargs mixed get_objects( string str, object player, int no_arr )

### PARAMETERS

    string str - The search string specifying the objects to locate.
    object [player] - The player object to use as a reference for searching.
    int [no_arr] - If specified, only a single object or 0 will be returned, otherwise an array of objects may be returned.

### RETURNS

    mixed - The located object(s), or 0 if not found.

## DESCRIPTION

Locates objects based on the specified search string, which can
include various search criteria and options.

## top_environment

## SYNOPSIS

    object top_environment(object ob)

### PARAMETERS

    object ob - The object to get the top-level environment of.

### RETURNS

    object - The top-level environment of the object.

## DESCRIPTION

Retrieves the top-level environment of the specified object,
traversing up through nested environments.

## find_ob

## SYNOPSIS

    varargs object find_ob(mixed ob, mixed cont, function f)

### PARAMETERS

    mixed ob - The object or name of the object to find.
    mixed [cont] - The container or environment to search within. Defaults to the previous object.
    function [f] - An optional function to further filter the search.

### RETURNS

    object - The found object, or 0 if not found.

## DESCRIPTION

Searches for an object within a container or environment
using the specified criteria.

## getoid

## SYNOPSIS

    int getoid(object ob)

### PARAMETERS

    object ob - The object to get the ID of.

### RETURNS

    int - The unique object ID.

## DESCRIPTION

Retrieves the unique object ID of the given object.

## get_object

## SYNOPSIS

    varargs object get_object( string str, object player )

### PARAMETERS

    string str - The name of the object to locate.
    object [player] - The player object to use as a reference for searching.

### RETURNS

    object - The located object, or 0 if not found.

## DESCRIPTION

Attempts to locate an object by the given name and returns the
object pointer if found.

