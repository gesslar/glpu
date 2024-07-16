# distinct_array

## SYNOPSIS

    mixed *distinct_array(mixed *arr)

### PARAMETERS

    mixed[] arr - An array of mixed types.

### RETURNS

    mixed[] - A new array with distinct elements from the input array.

## DESCRIPTION

Returns a new array containing the distinct elements of the input
array.


# remove_array_element

## SYNOPSIS

    varargs mixed *remove_array_element(mixed *arr, int start, int end)

### PARAMETERS

    mixed[] arr - The input array.
    int start - The starting index of elements to be removed.
    int [end] - The ending index of elements to be removed. Defaults to start if not specified.

### RETURNS

    mixed[] - A new array with specified elements removed.

## DESCRIPTION

Returns a new array containing the elements of the input array
from index 0 to start-1, and from end+1 to the end of the input
array. If start is greater than end, the new array will contain
all the elements of the input array.


# reverse_array

## SYNOPSIS

    mixed *reverse_array(mixed *arr)

### PARAMETERS

    mixed[] arr - The input array.

### RETURNS

    mixed[] - A new array with elements in reverse order.

## DESCRIPTION

Returns a new array with the elements of the input array in
reverse order.


# splice

## SYNOPSIS

    varargs mixed *splice(mixed *arr, int start, int delete_count, mixed *items_to_add)

### PARAMETERS

    mixed[] arr - The array from which elements will be removed and to which new elements may be added.
    int start - The zero-based index at which to start changing the array. If negative, it will begin that many elements from the end.
    int delete_count - The number of elements to remove from the array, starting from the index specified by start. If delete_count is 0, no elements are removed.
    mixed[] [items_to_add] - An array of elements to add to the array at the start index. Can be omitted or passed as null if no elements are to be added.

### RETURNS

    mixed[] - A new array reflecting the desired modifications.

## DESCRIPTION

Modifies the content of an array by removing existing elements
and/or adding new elements. Returns a new array with the
modifications.


# base64_decode

## SYNOPSIS

    string base64_decode(string source)

### PARAMETERS

    string source - The Base64 encoded string to be decoded.

### RETURNS

    string - The decoded string.

## DESCRIPTION

Decodes a given Base64 encoded string.


# base64_encode

## SYNOPSIS

    string base64_encode(mixed source_str)

### PARAMETERS

    mixed source_str - The string or buffer to be encoded.

### RETURNS

    string - The Base64 encoded string.

## DESCRIPTION

Encodes a given string or buffer into Base64 format.


# data_del

## SYNOPSIS

    int data_del(string file, string key)

### PARAMETERS

    string file - The file to modify.
    string key - The key to delete.

### RETURNS

    int - 1 if the key was found and deleted, 0 otherwise.

## DESCRIPTION

Deletes the key-value pair from the file.


# data_inc

## SYNOPSIS

    varargs int data_inc(string file, string key, int inc)

### PARAMETERS

    string file - The file to modify.
    string key - The key to increment the value for.
    int inc - The amount to increment by.

### RETURNS

    int - The new value after incrementing.

## DESCRIPTION

Increments the value associated with the given key in the file
by the specified amount. If the key does not exist, it is
created with the increment value.


# data_value

## SYNOPSIS

    varargs mixed data_value(string file, string key, mixed def)

### PARAMETERS

    string file - The file to read from.
    string key - The key to search for.
    mixed [def] - The default value to return if the key is not found.

### RETURNS

    mixed - The value associated with the key, or the default value if the key is not found.

## DESCRIPTION

Retrieves the value associated with a given key from a file.


# data_write

## SYNOPSIS

    varargs void data_write(string file, string key, mixed data...)

### PARAMETERS

    string file - The file to write to.
    string key - The key to write.
    mixed data - The value(s) to write.

## DESCRIPTION

Writes a key-value pair to a file. If the key already exists,
the value is updated.


# get_long

## SYNOPSIS

    string get_long(object ob, int extras: (: 1 :))

### PARAMETERS

    object ob - The object to get the long description of.
    int [extras=1] - Whether to include extra long descriptions. Defaults to 1 (include extras).

### RETURNS

    string - The long description of the object, including any extra long descriptions.

## DESCRIPTION

Returns the long description of an object, optionally
including extra long descriptions.


# get_short

## SYNOPSIS

    string get_short(object ob, int extras: (: 1 :))

### PARAMETERS

    object ob - The object to get the short description of.
    int [extras=1] - Whether to include extra short descriptions. Defaults to 1 (include extras).

### RETURNS

    string - The short description of the object, including any extra short descriptions.

## DESCRIPTION

Returns the short description of an object, optionally
including extra short descriptions in parentheses.


# assure_dir

## SYNOPSIS

    mixed assure_dir(string path)

### PARAMETERS

    string path - The path of the directory to ensure.

### RETURNS

    int - 1 if the directory exists or was created successfully, otherwise 0.

## DESCRIPTION

Ensures that a directory exists by creating it and its parent
directories if necessary.


# query_directory

## SYNOPSIS

    string query_directory(object ob)

### PARAMETERS

    object ob - The object to query the directory of.

### RETURNS

    string - The directory path of the object.

## DESCRIPTION

Returns the directory of the given object. If no object is
provided, it defaults to the previous object.


# cap_significant_words

## SYNOPSIS

    varargs string cap_significant_words(string str, int title)

### PARAMETERS

    string str - The string to capitalize.
    int [title=0] - Whether to capitalize the first word as a title.

### RETURNS

    string - The string with significant words capitalized.

## DESCRIPTION

Capitalizes significant words in a string, ignoring certain
insignificant words. Optionally capitalizes the first word
as a title.


# cap_words

## SYNOPSIS

    string cap_words(string str)

### PARAMETERS

    string str - The string to capitalize.

### RETURNS

    string - The capitalized string.

## DESCRIPTION

Capitalizes the first letter of each word in a string.


# objective

## SYNOPSIS

    string objective(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The objective pronoun.

## DESCRIPTION

Returns the objective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.


# possessive

## SYNOPSIS

    string possessive(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The possessive adjective.

## DESCRIPTION

Returns the possessive adjective corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.


# possessive_noun

## SYNOPSIS

    string possessive_noun(mixed ob)

### PARAMETERS

    mixed ob - The object or string to convert to possessive form.

### RETURNS

    string - The possessive form of the noun.

## DESCRIPTION

Returns the possessive form of a noun. If the noun ends with 's',
it adds an apostrophe; otherwise, it adds 's.


# possessive_pronoun

## SYNOPSIS

    string possessive_pronoun(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The possessive pronoun.

## DESCRIPTION

Returns the possessive pronoun corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.


# reflexive

## SYNOPSIS

    string reflexive(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The reflexive pronoun.

## DESCRIPTION

Returns the reflexive pronoun corresponding to the object's
gender. Defaults to "itself" for non-string or unknown gender.


# subjective

## SYNOPSIS

    string subjective(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The subjective pronoun.

## DESCRIPTION

Returns the subjective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.


