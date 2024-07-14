# get_short

## SYNOPSIS

    string get_short(object ob, int extras: (: 1 :))

### PARAMETERS

    object ob - The object to get the short description of.
    int [extras=1] - Whether to include extra short descriptions. Defaults to 1 (include extras).

### RETURNS

    string - - The short description of the object, including any extra short descriptions.

## DESCRIPTION

Returns the short description of an object, optionally
including extra short descriptions in parentheses.
