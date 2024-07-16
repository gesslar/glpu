## time_frac

## SYNOPSIS

    varargs float time_frac(int nanoseconds)

### PARAMETERS

    int nanoseconds - The time value in nanoseconds. Defaults to the current time in nanoseconds if not provided.

### RETURNS

    float - The time value converted to a fractional value in seconds.

## DESCRIPTION

Converts a time value from nanoseconds to a fractional value in seconds.
This function takes a time value in nanoseconds and converts it to
a fractional value representing seconds by dividing the input by 1,000,000,000.

## time_ms

## SYNOPSIS

    varargs int time_ms(int nanoseconds)

### PARAMETERS

    int nanoseconds - The time value in nanoseconds. Defaults to the current time in nanoseconds if not provided.

### RETURNS

    int - The time value converted to milliseconds.

## DESCRIPTION

Converts a time value from nanoseconds to milliseconds.
This function takes a time value in nanoseconds and converts it
to milliseconds by dividing the input by 1,000,000.

## ltime

## SYNOPSIS

    varargs string ltime(int x, int brief)

### PARAMETERS

    int [x=time()] - The timestamp to format. Defaults to the current time.
    int [brief=0] - If set to 1, returns a brief time format (HH:MM). Otherwise, returns a full time format (HH:MM:SS).

### RETURNS

    string - The formatted time string.

## DESCRIPTION

Returns a formatted time string.

## ldate

## SYNOPSIS

    varargs string ldate(int x, int brief)

### PARAMETERS

    int [x=time()] - The timestamp to format. Defaults to the current time.
    int [brief=0] - If set to 1, returns a brief date format (MM-DD). Otherwise, returns a full date format (YYYY-MM-DD).

### RETURNS

    string - The formatted date string.

## DESCRIPTION

Returns a formatted date string.
