---
title: system
---
# system.c

## mud_config

### Synopsis

```c
mixed mud_config(string str)
```

### Parameters

* `string str` - The configuration key to retrieve.

### Returns

`mixed` - The configuration value.

### Description

Retrieves a specific configuration value from the MUD config.

## _info

### Synopsis

```c
varargs int _info(object tp, string str, mixed args...)
varargs int _info(string str, mixed args...)
```

### Parameters

* `string str` - The informational message.
* `mixed [args]` - Optional arguments to format the message.

### Returns

`int` - Always returns 1, unless there is no previous object.

### Description

Provides an informational message, optionally formatted with
arguments. If no object is provided, the message will be sent
to this_body(). If no object is found, the message will be
sent to the debug log.

## mud_name

### Synopsis

```c
string mud_name()
```

### Returns

`string` - The name of the MUD.

### Description

Returns the name of the MUD.

## _ok

### Synopsis

```c
varargs int _ok(object tp, string str, mixed args...)
varargs int _ok(string str, mixed args...)
```

### Parameters

* `string str` - The confirmation message.
* `mixed [args]` - Optional arguments to format the message.

### Returns

`int` - Always returns 1, unless there is no previous object.

### Description

Provides an confirmation message, optionally formatted with
arguments. If no object is provided, the message will be sent
to this_body(). If no object is found, the message will be
sent to the debug log.

## lib_name

### Synopsis

```c
string lib_name()
```

### Returns

`string` - The library name.

### Description

Returns the name of the library being used by the MUD.

## tmp_dir

### Synopsis

```c
string tmp_dir()
```

### Returns

`string` - The temporary directory.

### Description

Returns the directory where temporary files are stored.

## doc_dir

### Synopsis

```c
string doc_dir()
```

### Returns

`string` - The documentation directory.

### Description

Returns the directory where documentation files are stored.

## admin_email

### Synopsis

```c
string admin_email()
```

### Returns

`string` - The admin email address.

### Description

Returns the admin email address for the MUD.

## arch

### Synopsis

```c
string arch()
```

### Returns

`string` - The system architecture.

### Description

Returns the architecture of the system the MUD is running on.

## port

### Synopsis

```c
int port()
```

### Returns

`int` - The port number.

### Description

Returns the port number the MUD is running on.

## baselib_name

### Synopsis

```c
string baselib_name()
```

### Returns

`string` - The base library name.

### Description

Returns the name of the base library.

## driver_version

### Synopsis

```c
string driver_version()
```

### Returns

`string` - The driver version.

### Description

Returns the version of the MUD driver.

## lib_version

### Synopsis

```c
string lib_version()
```

### Returns

`string` - The library version.

### Description

Returns the version of the library being used by the MUD.

## baselib_version

### Synopsis

```c
string baselib_version()
```

### Returns

`string` - The base library version.

### Description

Returns the version of the base library.

## _warn

### Synopsis

```c
varargs int _warn(object tp, string str, mixed args...)
varargs int _warn(string str, mixed args...)
```

### Parameters

* `string str` - The warning message.
* `mixed [args]` - Optional arguments to format the message.

### Returns

`int` - Always returns 1, unless there is no previous object.

### Description

Provides an warning message, optionally formatted with
arguments. If no object is provided, the message will be sent
to this_body(). If no object is found, the message will be
sent to the debug log.

## _error

### Synopsis

```c
varargs int _error(object tp, string str, mixed args...)
varargs int _error(string str, mixed args...)
```

### Parameters

* `string str` - The error message.
* `mixed [args]` - Optional arguments to format the message.

### Returns

`int` - Always returns 1, unless there is no previous object.

### Description

Provides an error message, optionally formatted with
arguments. If no object is provided, the message will be sent
to this_body(). If no object is found, the message will be
sent to the debug log.

## open_status

### Synopsis

```c
string open_status()
```

### Returns

`string` - The open status of the MUD.

### Description

Returns the open status of the MUD.

## boot_number

### Synopsis

```c
int boot_number()
```

### Returns

`int` - The current boot number.

### Description

Returns the boot number of the MUD. This is the number of
times the MUD has been booted.

## log_dir

### Synopsis

```c
string log_dir()
```

### Returns

`string` - The log directory.

### Description

Returns the directory where log files are stored.

## _question

### Synopsis

```c
varargs int _question(object tp, string str, mixed args...)
varargs int _question(string str, mixed args...)
```

### Parameters

* `string str` - The question message.
* `mixed [args]` - Optional arguments to format the message.

### Returns

`int` - Always returns 1, unless there is no body object.

### Description

Provides a question message, optionally formatted with
arguments. If no object is provided, the message will be sent
to this_body(). If no object is found, the message will be
discarded.

