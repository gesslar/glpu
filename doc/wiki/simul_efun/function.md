## call_trace

## SYNOPSIS

    varargs string call_trace(int colour)

### PARAMETERS

    int [colour=0] - Whether to include ANSI colour codes. Defaults to 0 (no colour).

### RETURNS

    string - The formatted call stack trace.

## DESCRIPTION

Returns a formatted string of the current call stack trace.

## assemble_call_back

## SYNOPSIS

    mixed *assemble_call_back(mixed arg...)

### PARAMETERS

    mixed arg - The arguments to assemble into a callback.

### RETURNS

    mixed[] - The assembled callback.

## DESCRIPTION

Assembles a callback function from the provided arguments.
This function is used to create a callable structure that can be
invoked later. The callback can be either a method on an object or
a function. The assembled callback is returned as an array.
Usage:
- When you need to create a callback for an object method:
`assemble_call_back(object, "method", args...)`
- When you need to create a callback for a function:
`assemble_call_back(function, args...)`
The function performs the following steps:
1. Checks if the provided arguments form a valid array.
2. Determines the size of the arguments array.
3. Checks if the first argument is an object. If so, it verifies that
the second argument is a valid method name on the object.
4. If the first argument is a function, it creates a callback with the
function and any additional arguments.
5. Returns the assembled callback as an array.

## call_back

## SYNOPSIS

    mixed call_back(mixed cb, mixed new_arg...)

### PARAMETERS

    mixed cb - The callback to execute.
    mixed new_arg - The arguments to pass to the callback.

### RETURNS

    mixed - The result of the callback execution.

## DESCRIPTION

Executes a callback with the given arguments.

## valid_function

## SYNOPSIS

    int valid_function(mixed f)

### PARAMETERS

    mixed f - The function to check.

### RETURNS

    int - 1 if the function is valid, otherwise 0.

## DESCRIPTION

Checks if a given function is valid and not owned by a destructed
object.

