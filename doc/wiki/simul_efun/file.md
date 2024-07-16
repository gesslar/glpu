## file_owner

## SYNOPSIS

    string file_owner(string file)

### PARAMETERS

* `string file` - The path of the file to check.

## DESCRIPTION

Determines the owner of a file based on its path.

## dir_file

## SYNOPSIS

    string *dir_file(string path)

### PARAMETERS

* `string path` - The path to extract the components from.

## DESCRIPTION

Given a path, returns an array containing the directory and file
name components.

## temp_file

## SYNOPSIS

    varargs string temp_file(mixed arg)

### PARAMETERS

* `mixed arg` - The file or object to create a temporary file for.

## DESCRIPTION

Generates a temporary file name based on the provided argument.

## assure_file

## SYNOPSIS

    mixed assure_file(string file)

### PARAMETERS

* `string file` - The path of the file to ensure.

## DESCRIPTION

Given a file, ensures that the directory structure leading to
the file exists, creating directories as needed.

## tail

## SYNOPSIS

    varargs string tail(string path, int line_count)

### PARAMETERS

* `string path` - The path of the file to read.
* `int [line_count=25]` - The number of lines to read from the end of the file. Defaults to 25.

## DESCRIPTION

Returns the last few lines of a file, similar to the Unix
`tail` command.

## implode_file

## SYNOPSIS

    varargs void implode_file(string file, string *lines, int overwrite)

### PARAMETERS

* `string file` - The path of the file to write to.
* `string[] lines` - The array of lines to write.
* `int [overwrite=0]` - Whether to overwrite the existing content. Defaults to 0 (append).

## DESCRIPTION

Writes an array of lines to a specified file, optionally
overwriting the existing content.

## log_file

## SYNOPSIS

    varargs int log_file(string file, string str, mixed arg...)

### PARAMETERS

* `string file` - The name of the log file.
* `string str` - The log message to write.
* `mixed [arg]` - Additional arguments to include in the log message.

## DESCRIPTION

Writes a log message to a specified log file.

## explode_file

## SYNOPSIS

    string *explode_file(string file)

### PARAMETERS

* `string file` - The path of the file to read.

## DESCRIPTION

Reads a file and returns its content as an array of lines,
excluding comment lines and empty lines.

## query_file_name

## SYNOPSIS

    string query_file_name(object ob)

### PARAMETERS

* `object ob` - The object to query the file name of.

## DESCRIPTION

Returns the name of the file corresponding to a given object.

