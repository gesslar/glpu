# tail

## SYNOPSIS

    varargs string tail(string path, int line_count)

### PARAMETERS

    string path - The path of the file to read.
    int [line_count=25] - The number of lines to read from the end of the file. Defaults to 25.

### RETURNS

    string - - The last few lines of the file.

## DESCRIPTION

Returns the last few lines of a file, similar to the Unix
`tail` command.
