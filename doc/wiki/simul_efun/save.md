## object_save_directory

## SYNOPSIS

    string object_save_directory(object ob)

### PARAMETERS

    object ob - The object to get the save directory for.

### RETURNS

    string - The save directory path for the object.

## DESCRIPTION

Returns the save directory path for the specified object.

## assure_object_save_dir

## SYNOPSIS

    string assure_object_save_dir(object ob)

### PARAMETERS

    object ob - The object to ensure the save directory for.

### RETURNS

    string - The save directory path for the object, or 0 if creation failed.

## DESCRIPTION

Ensures the save directory for the specified object exists,
creating it if necessary.

## object_save_file

## SYNOPSIS

    string object_save_file(object ob)

### PARAMETERS

    object ob - The object to get the save file for.

### RETURNS

    string - The save file path for the object.

## DESCRIPTION

Returns the save file path for the specified object.
