# log_file

## SYNOPSIS

    varargs int log_file(string file, string str, mixed arg...)

### PARAMETERS

    string file - The name of the log file.
    string str - The log message to write.
    mixed [arg] - Additional arguments to include in the log message.

### RETURNS

    int - - 1 if the log message was written successfully, otherwise 0.

## DESCRIPTION

Writes a log message to a specified log file.
