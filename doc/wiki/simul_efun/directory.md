## query_directory

## SYNOPSIS

    string query_directory(object ob)

### PARAMETERS

* `object ob` - The object to query the directory of.

## DESCRIPTION

Returns the directory of the given object. If no object is
provided, it defaults to the previous object.

## assure_dir

## SYNOPSIS

    mixed assure_dir(string path)

### PARAMETERS

* `string path` - The path of the directory to ensure.

## DESCRIPTION

Ensures that a directory exists by creating it and its parent
directories if necessary.

