## mud_config

## SYNOPSIS

```c
mixed mud_config(string str)
```

### PARAMETERS

* `string str` - The configuration key to retrieve.

## DESCRIPTION

Retrieves a specific configuration value from the MUD config.

## mud_name

## SYNOPSIS

```c
string mud_name()
```

## DESCRIPTION

Returns the name of the MUD.

## _ok

## SYNOPSIS

```c
int _ok(mixed str, mixed args...)
```

### PARAMETERS

* `string str` - The informational message.
* `mixed [args]` - Optional arguments to format the message.

## DESCRIPTION

Provides an informational message, optionally formatted with arguments.

## lib_name

## SYNOPSIS

```c
string lib_name()
```

## DESCRIPTION

Returns the name of the library being used by the MUD.

## tmp_dir

## SYNOPSIS

```c
string tmp_dir()
```

## DESCRIPTION

Returns the directory where temporary files are stored.

## admin_email

## SYNOPSIS

```c
string admin_email()
```

## DESCRIPTION

Returns the admin email address for the MUD.

## arch

## SYNOPSIS

```c
string arch()
```

## DESCRIPTION

Returns the architecture of the system the MUD is running on.

## port

## SYNOPSIS

```c
int port()
```

## DESCRIPTION

Returns the port number the MUD is running on.

## baselib_name

## SYNOPSIS

```c
string baselib_name()
```

## DESCRIPTION

Returns the name of the base library.

## driver_version

## SYNOPSIS

```c
string driver_version()
```

## DESCRIPTION

Returns the version of the MUD driver.

## lib_version

## SYNOPSIS

```c
string lib_version()
```

## DESCRIPTION

Returns the version of the library being used by the MUD.

## baselib_version

## SYNOPSIS

```c
string baselib_version()
```

## DESCRIPTION

Returns the version of the base library.

## _warn

## SYNOPSIS

```c
int _warn(mixed str, mixed args...)
```

### PARAMETERS

* `string str` - The warning message.
* `mixed [args]` - Optional arguments to format the message.

## DESCRIPTION

Provides a warning message, optionally formatted with arguments.

## _error

## SYNOPSIS

```c
int _error(mixed str, mixed args...)
```

### PARAMETERS

* `string str` - The error message.
* `mixed [args]` - Optional arguments to format the message.

## DESCRIPTION

Provides an error message, optionally formatted with arguments.

## open_status

## SYNOPSIS

```c
string open_status()
```

## DESCRIPTION

Returns the open status of the MUD.

## log_dir

## SYNOPSIS

```c
string log_dir()
```

## DESCRIPTION

Returns the directory where log files are stored.

