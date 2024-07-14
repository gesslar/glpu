# assure_object_save_dir

## SYNOPSIS

    string assure_object_save_dir(object ob)

### PARAMETERS

    object ob - The object to ensure the save directory for.

### RETURNS

    string - - The save directory path for the object, or 0 if creation failed.

## DESCRIPTION

Ensures the save directory for the specified object exists,
creating it if necessary.
