---
title: function
---
# function.c

## call_trace

### Synopsis

```c
varargs string call_trace(int colour)
```

### Parameters

* `int [colour=0]` - Whether to include colour codes. Defaults to 0 (no colour).

### Returns

`string` - The formatted call stack trace.

### Description

Returns a formatted string of the current call stack trace.

## delay_act

### Synopsis

```c
varargs int delay_act(string act, float delay, mixed *cb)
```

### Parameters

* `string action` - The action to delay.
* `float delay` - The amount of time to delay the action.
* `mixed* cb` - The callback to execute after the delay.

### Returns

`int` - The ID of the delayed action.

### Description

Delays an action for a specified amount of time.

## assert

### Synopsis

```c
varargs void assert(mixed statement, string message)
```

### Parameters

* `mixed statement` - The statement to assert.
* `string message` - The message to display if the condition is false.

### Description

Asserts that a statement is true. If the statement is false, it
will throw an error with the given message. If no message is
provided, it will use a default message.

## assemble_call_back

### Synopsis

```c
mixed *assemble_call_back(mixed arg...)
```

### Parameters

* `mixed arg` - The arguments to assemble into a callback.

### Returns

`mixed*` - The assembled callback.

### Description

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

### Synopsis

```c
mixed call_back(mixed cb, mixed new_arg...)
```

### Parameters

* `mixed cb` - The callback to execute.
* `mixed new_arg` - The arguments to pass to the callback.

### Returns

`mixed` - The result of the callback execution.

### Description

Executes a callback with the given arguments.

## call_if

### Synopsis

```c
varargs mixed call_if(mixed ob, string func, mixed arg...)
```

### Parameters

* `mixed ob` - The object to call the function on.
* `string func` - The name of the function to call.
* `mixed arg` - The argument to pass to the function.

### Returns

`mixed` - The return value of the function, or null if the function does not exist.

### Description

Calls the specified function on the given object if it exists.

## valid_function

### Synopsis

```c
int valid_function(mixed f)
```

### Parameters

* `mixed f` - The function to check.

### Returns

`int` - 1 if the function is valid, otherwise 0.

### Description

Checks if a given function is valid and not owned by a destructed
object.

