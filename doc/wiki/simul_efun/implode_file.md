# implode_file

## SYNOPSIS

    varargs void implode_file(string file, string *lines, int overwrite)

### PARAMETERS

    string file - The path of the file to write to.
    string[] lines - The array of lines to write.
    int [overwrite=0] - Whether to overwrite the existing content. Defaults to 0 (append).

## DESCRIPTION

Writes an array of lines to a specified file, optionally
overwriting the existing content.
