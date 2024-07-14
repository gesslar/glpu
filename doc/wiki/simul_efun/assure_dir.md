# assure_dir

## SYNOPSIS

    int assure_dir(string path)

### PARAMETERS

    string path - The path of the directory to ensure.

### RETURNS

    int - - 1 if the directory exists or was created successfully, otherwise 0.

## DESCRIPTION

Ensures that a directory exists by creating it and its parent
directories if necessary.
