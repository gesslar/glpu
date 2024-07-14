# find_ob

## SYNOPSIS

    varargs object find_ob(mixed ob, mixed cont, function f)

### PARAMETERS

    mixed ob - The object or name of the object to find.
    mixed [cont] - The container or environment to search within. Defaults to the previous object.
    function [f] - An optional function to further filter the search.

### RETURNS

    object - - The found object, or 0 if not found.

## DESCRIPTION

Searches for an object within a container or environment
using the specified criteria.
