## mud_config

## SYNOPSIS

    mixed mud_config(string str)

### PARAMETERS

    string str - The configuration key to retrieve.

### RETURNS

    mixed - The configuration value.

## DESCRIPTION

Retrieves a specific configuration value from the MUD config.

## mud_name

## SYNOPSIS

    string mud_name()

### RETURNS

    string - The name of the MUD.

## DESCRIPTION

Returns the name of the MUD.

## _ok

## SYNOPSIS

    int _ok(mixed str, mixed args...)

### PARAMETERS

    string str - The informational message.
    mixed [args] - Optional arguments to format the message.

### RETURNS

    int - Always returns 1, unless there is no previous object.

## DESCRIPTION

Provides an informational message, optionally formatted with arguments.

## lib_name

## SYNOPSIS

    string lib_name()

### RETURNS

    string - The library name.

## DESCRIPTION

Returns the name of the library being used by the MUD.

## tmp_dir

## SYNOPSIS

    string tmp_dir()

### RETURNS

    string - The temporary directory.

## DESCRIPTION

Returns the directory where temporary files are stored.

## admin_email

## SYNOPSIS

    string admin_email()

### RETURNS

    string - The admin email address.

## DESCRIPTION

Returns the admin email address for the MUD.

## arch

## SYNOPSIS

    string arch()

### RETURNS

    string - The system architecture.

## DESCRIPTION

Returns the architecture of the system the MUD is running on.

## port

## SYNOPSIS

    int port()

### RETURNS

    int - The port number.

## DESCRIPTION

Returns the port number the MUD is running on.

## baselib_name

## SYNOPSIS

    string baselib_name()

### RETURNS

    string - The base library name.

## DESCRIPTION

Returns the name of the base library.

## driver_version

## SYNOPSIS

    string driver_version()

### RETURNS

    string - The driver version.

## DESCRIPTION

Returns the version of the MUD driver.

## lib_version

## SYNOPSIS

    string lib_version()

### RETURNS

    string - The library version.

## DESCRIPTION

Returns the version of the library being used by the MUD.

## baselib_version

## SYNOPSIS

    string baselib_version()

### RETURNS

    string - The base library version.

## DESCRIPTION

Returns the version of the base library.

## _warn

## SYNOPSIS

    int _warn(mixed str, mixed args...)

### PARAMETERS

    string str - The warning message.
    mixed [args] - Optional arguments to format the message.

### RETURNS

    int - Always returns 1, unless there is no previous object.

## DESCRIPTION

Provides a warning message, optionally formatted with arguments.

## _error

## SYNOPSIS

    int _error(mixed str, mixed args...)

### PARAMETERS

    string str - The error message.
    mixed [args] - Optional arguments to format the message.

### RETURNS

    int - Always returns 1, unless there is no previous object.

## DESCRIPTION

Provides an error message, optionally formatted with arguments.

## open_status

## SYNOPSIS

    string open_status()

### RETURNS

    string - The open status of the MUD.

## DESCRIPTION

Returns the open status of the MUD.

## log_dir

## SYNOPSIS

    string log_dir()

### RETURNS

    string - The log directory.

## DESCRIPTION

Returns the directory where log files are stored.

