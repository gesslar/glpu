# time_frac

## SYNOPSIS

    varargs float time_frac(int nanoseconds)

### PARAMETERS

    int nanoseconds - The time value in nanoseconds. Defaults to the current time in nanoseconds if not provided.

### RETURNS

    float - - The time value converted to a fractional value in seconds.

## DESCRIPTION

Converts a time value from nanoseconds to a fractional value in seconds.
This function takes a time value in nanoseconds and converts it to
a fractional value representing seconds by dividing the input by 1,000,000,000.
