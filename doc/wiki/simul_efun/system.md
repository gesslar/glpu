# mud_config

## Synopsis

```c
mixed mud_config(string str)
```

### Parameters

* `string str` - The configuration key to retrieve.

## Description

Retrieves a specific configuration value from the MUD config.

# mud_name

## Synopsis

```c
string mud_name()
```

## Description

Returns the name of the MUD.

# _ok

## Synopsis

```c
int _ok(mixed str, mixed args...)
```

### Parameters

* `string str` - The informational message.
* `mixed [args]` - Optional arguments to format the message.

## Description

Provides an informational message, optionally formatted with arguments.

# lib_name

## Synopsis

```c
string lib_name()
```

## Description

Returns the name of the library being used by the MUD.

# tmp_dir

## Synopsis

```c
string tmp_dir()
```

## Description

Returns the directory where temporary files are stored.

# admin_email

## Synopsis

```c
string admin_email()
```

## Description

Returns the admin email address for the MUD.

# arch

## Synopsis

```c
string arch()
```

## Description

Returns the architecture of the system the MUD is running on.

# port

## Synopsis

```c
int port()
```

## Description

Returns the port number the MUD is running on.

# baselib_name

## Synopsis

```c
string baselib_name()
```

## Description

Returns the name of the base library.

# driver_version

## Synopsis

```c
string driver_version()
```

## Description

Returns the version of the MUD driver.

# lib_version

## Synopsis

```c
string lib_version()
```

## Description

Returns the version of the library being used by the MUD.

# baselib_version

## Synopsis

```c
string baselib_version()
```

## Description

Returns the version of the base library.

# _warn

## Synopsis

```c
int _warn(mixed str, mixed args...)
```

### Parameters

* `string str` - The warning message.
* `mixed [args]` - Optional arguments to format the message.

## Description

Provides a warning message, optionally formatted with arguments.

# _error

## Synopsis

```c
int _error(mixed str, mixed args...)
```

### Parameters

* `string str` - The error message.
* `mixed [args]` - Optional arguments to format the message.

## Description

Provides an error message, optionally formatted with arguments.

# open_status

## Synopsis

```c
string open_status()
```

## Description

Returns the open status of the MUD.

# log_dir

## Synopsis

```c
string log_dir()
```

## Description

Returns the directory where log files are stored.

