## shout

## SYNOPSIS

    varargs void shout(string msg, mixed exclude)

### PARAMETERS

    string msg - The message to write.
    mixed exclude - The object or array of objects to exclude from the message.

### RETURNS


## DESCRIPTION

This function is an override of the shout() efun. It will
write a message to all users using the message() efun.

## set_privs

## SYNOPSIS

    void set_privs(object ob, string privs)

### PARAMETERS

    object ob - The object to destruct.

### RETURNS


## DESCRIPTION

This function is an override for the efun destruct(). It checks
if the object calling the function has admin privs. If it does,
it will execute the function as normal. If it does not, it will
return 0.

## ctime

## SYNOPSIS

    varargs string ctime(int x)

### PARAMETERS

    int x - The time to convert to a string.

### RETURNS

    string - The time in string form.

## DESCRIPTION

This function is an override of the efun ctime(). It will
return the current time in the format "YYYY-MM-DD HH:MM:SS".
It also will use time() if no argument is provided.

## tell_object

## SYNOPSIS

    varargs void tell_object(mixed ob, mixed msg, mixed type)

### PARAMETERS

    object ob - The object to write the message to.
    string msg - The message to write.
    string type - The message class of message to write.

### RETURNS


## DESCRIPTION

This function is an override of the tell_object() efun. It will
write a message to the object using the message() efun.

## write

## SYNOPSIS

    void write(string msg)

### PARAMETERS

    string msg - The message to write.

### RETURNS


## DESCRIPTION

This function is an override of the write() efun. It will
write a message to the player using the message() efun
if this_player(), otherwise it will use the debug_message()
efun.

## this_body

## SYNOPSIS

    object this_body()

### RETURNS

    object - The this_player().

## DESCRIPTION

This is an override of the efun this_body(). It will return
the this_player().

## query_num

## SYNOPSIS

    varargs string query_num(int x, int many)

### PARAMETERS

    int x - The number to convert to a string.
    int many - If true, the number will be returned in plural form.

### RETURNS

    string - The number in string form.

## DESCRIPTION

This function is an override of query_num(). It will return
the number in string form.

## say

## SYNOPSIS

    varargs void say(string msg, mixed exclude)

### PARAMETERS

    string msg - The message to write.
    mixed exclude - The object or array of objects to exclude from the message.

### RETURNS


## DESCRIPTION

This function is an override of the say() efun. It will
write a message to the room using the message() efun.

## shutdown

## SYNOPSIS

    void shutdown( int how )

### PARAMETERS

    int how - The type of shutdown to perform.

### RETURNS


## DESCRIPTION

This function is an override for the efun shutdown(). It checks
if the object calling the function has admin privs. If it does,
it will execute the function as normal. If it does not, it will
return 0.

## tell_room

## SYNOPSIS

    varargs void tell_room(mixed room, string msg, mixed exclude)

### PARAMETERS

    mixed room - The room to write the message to.
    string msg - The message to write.
    mixed exclude - The object or array of objects to exclude from the message.

### RETURNS


## DESCRIPTION

This function is an override of the tell_room() efun. It will
write a message to the room using the message() efun.

## exec

## SYNOPSIS

    int exec( object to, object from )

### PARAMETERS

    object to - The object to exec to.
    object from - The object to exec from.

### RETURNS

    int - 1 if the exec was successful, 0 if it was not.

## DESCRIPTION

This function is an override for the efun exec(). It checks
if the object calling the function has admin privs. If it does,
it will
execute the function as normal. If it does not, it will return 0.

## userp

## SYNOPSIS

    varargs int userp(object ob)

### PARAMETERS

    object ob - The object to check.

### RETURNS

    int - 1 if the object is a user, otherwise 0.

## DESCRIPTION

This function is an override of the userp() efun. Unlike the
efun, this function accepts a null argument and will check
if the previous object is a user if no argument is provided.

## this_user

## SYNOPSIS

    object this_user()

### RETURNS

    object - The this_player()->query_user().

## DESCRIPTION

This is an override of the efun this_user(). It will return
the this_player()->query_user().

## reverse_array

## SYNOPSIS

    mixed *reverse_array(mixed *arr)

### PARAMETERS

    mixed[] arr - The input array.

### RETURNS

    mixed[] - A new array with elements in reverse order.

## DESCRIPTION

Returns a new array with the elements of the input array in
reverse order.

## remove_array_element

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

## distinct_array

## SYNOPSIS

    mixed *distinct_array(mixed *arr)

### PARAMETERS

    mixed[] arr - An array of mixed types.

### RETURNS

    mixed[] - A new array with distinct elements from the input array.

## DESCRIPTION

Returns a new array containing the distinct elements of the input
array.

## splice

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

## base64_decode

## SYNOPSIS

    string base64_decode(string source)

### PARAMETERS

    string source - The Base64 encoded string to be decoded.

### RETURNS

    string - The decoded string.

## DESCRIPTION

Decodes a given Base64 encoded string.

## base64_encode

## SYNOPSIS

    string base64_encode(mixed source_str)

### PARAMETERS

    mixed source_str - The string or buffer to be encoded.

### RETURNS

    string - The Base64 encoded string.

## DESCRIPTION

Encodes a given string or buffer into Base64 format.

## data_inc

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

## data_del

## SYNOPSIS

    int data_del(string file, string key)

### PARAMETERS

    string file - The file to modify.
    string key - The key to delete.

### RETURNS

    int - 1 if the key was found and deleted, 0 otherwise.

## DESCRIPTION

Deletes the key-value pair from the file.

## data_value

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

## data_write

## SYNOPSIS

    varargs void data_write(string file, string key, mixed data...)

### PARAMETERS

    string file - The file to write to.
    string key - The key to write.
    mixed data - The value(s) to write.

## DESCRIPTION

Writes a key-value pair to a file. If the key already exists,
the value is updated.

## get_long

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

## get_short

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

## query_directory

## SYNOPSIS

    string query_directory(object ob)

### PARAMETERS

    object ob - The object to query the directory of.

### RETURNS

    string - The directory path of the object.

## DESCRIPTION

Returns the directory of the given object. If no object is
provided, it defaults to the previous object.

## assure_dir

## SYNOPSIS

    mixed assure_dir(string path)

### PARAMETERS

    string path - The path of the directory to ensure.

### RETURNS

    int - 1 if the directory exists or was created successfully, otherwise 0.

## DESCRIPTION

Ensures that a directory exists by creating it and its parent
directories if necessary.

## cap_words

## SYNOPSIS

    string cap_words(string str)

### PARAMETERS

    string str - The string to capitalize.

### RETURNS

    string - The capitalized string.

## DESCRIPTION

Capitalizes the first letter of each word in a string.

## subjective

## SYNOPSIS

    string subjective(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The subjective pronoun.

## DESCRIPTION

Returns the subjective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.

## possessive_noun

## SYNOPSIS

    string possessive_noun(mixed ob)

### PARAMETERS

    mixed ob - The object or string to convert to possessive form.

### RETURNS

    string - The possessive form of the noun.

## DESCRIPTION

Returns the possessive form of a noun. If the noun ends with 's',
it adds an apostrophe; otherwise, it adds 's.

## cap_significant_words

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

## possessive

## SYNOPSIS

    string possessive(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The possessive adjective.

## DESCRIPTION

Returns the possessive adjective corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.

## possessive_pronoun

## SYNOPSIS

    string possessive_pronoun(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The possessive pronoun.

## DESCRIPTION

Returns the possessive pronoun corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.

## objective

## SYNOPSIS

    string objective(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The objective pronoun.

## DESCRIPTION

Returns the objective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.

## reflexive

## SYNOPSIS

    string reflexive(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The reflexive pronoun.

## DESCRIPTION

Returns the reflexive pronoun corresponding to the object's
gender. Defaults to "itself" for non-string or unknown gender.

## file_exists

## SYNOPSIS

    int file_exists(string file)

### PARAMETERS

    string file - The name of the file to check.

### RETURNS

    int - 1 if the file exists, otherwise 0.

## DESCRIPTION

Checks if a file exists.

## user_exists

## SYNOPSIS

    int user_exists(string user)

### PARAMETERS

    string user - The username to check.

### RETURNS

    int - 1 if the user data file exists, otherwise 0.

## DESCRIPTION

Checks if a user data file exists.

## cfile_exists

## SYNOPSIS

    int cfile_exists(string file)

### PARAMETERS

    string file - The base name of the file to check.

### RETURNS

    int - 1 if the compiled file exists, otherwise 0.

## DESCRIPTION

Checks if a compiled file (.c) exists.

## ofile_exists

## SYNOPSIS

    int ofile_exists(string file)

### PARAMETERS

    string file - The base name of the file to check.

### RETURNS

    int - 1 if the save file exists, otherwise 0.

## DESCRIPTION

Checks if a save file exists.

## directory_exists

## SYNOPSIS

    int directory_exists(string dirname)

### PARAMETERS

    string dirname - The name of the directory to check.

### RETURNS

    int - 1 if the directory exists, otherwise 0.

## DESCRIPTION

Checks if a directory exists.

