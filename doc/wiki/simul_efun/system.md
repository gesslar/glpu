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

## file_owner

## SYNOPSIS

    string file_owner(string file)

### PARAMETERS

    string file - The path of the file to check.

### RETURNS

    string - The owner of the file, or 0 if not found.

## DESCRIPTION

Determines the owner of a file based on its path.

## dir_file

## SYNOPSIS

    string *dir_file(string path)

### PARAMETERS

    string path - The path to extract the components from.

### RETURNS

    string[] - An array containing the directory and file name components.

## DESCRIPTION

Given a path, returns an array containing the directory and file
name components.

## temp_file

## SYNOPSIS

    varargs string temp_file(mixed arg)

### PARAMETERS

    mixed arg - The file or object to create a temporary file for.

### RETURNS

    string - The path to the temporary file.

## DESCRIPTION

Generates a temporary file name based on the provided argument.

## assure_file

## SYNOPSIS

    mixed assure_file(string file)

### PARAMETERS

    string file - The path of the file to ensure.

## DESCRIPTION

Given a file, ensures that the directory structure leading to
the file exists, creating directories as needed.

## tail

## SYNOPSIS

    varargs string tail(string path, int line_count)

### PARAMETERS

    string path - The path of the file to read.
    int [line_count=25] - The number of lines to read from the end of the file. Defaults to 25.

### RETURNS

    string - The last few lines of the file.

## DESCRIPTION

Returns the last few lines of a file, similar to the Unix
`tail` command.

## implode_file

## SYNOPSIS

    varargs void implode_file(string file, string *lines, int overwrite)

### PARAMETERS

    string file - The path of the file to write to.
    string[] lines - The array of lines to write.
    int [overwrite=0] - Whether to overwrite the existing content. Defaults to 0 (append).

## DESCRIPTION

Writes an array of lines to a specified file, optionally
overwriting the existing content.

## log_file

## SYNOPSIS

    varargs int log_file(string file, string str, mixed arg...)

### PARAMETERS

    string file - The name of the log file.
    string str - The log message to write.
    mixed [arg] - Additional arguments to include in the log message.

### RETURNS

    int - 1 if the log message was written successfully, otherwise 0.

## DESCRIPTION

Writes a log message to a specified log file.

## explode_file

## SYNOPSIS

    string *explode_file(string file)

### PARAMETERS

    string file - The path of the file to read.

### RETURNS

    string[] - An array of lines from the file.

## DESCRIPTION

Reads a file and returns its content as an array of lines,
excluding comment lines and empty lines.

## query_file_name

## SYNOPSIS

    string query_file_name(object ob)

### PARAMETERS

    object ob - The object to query the file name of.

### RETURNS

    string - The name of the file corresponding to the object.

## DESCRIPTION

Returns the name of the file corresponding to a given object.

## call_trace

## SYNOPSIS

    varargs string call_trace(int colour)

### PARAMETERS

    int [colour=0] - Whether to include ANSI colour codes. Defaults to 0 (no colour).

### RETURNS

    string - The formatted call stack trace.

## DESCRIPTION

Returns a formatted string of the current call stack trace.

## assemble_call_back

## SYNOPSIS

    mixed *assemble_call_back(mixed arg...)

### PARAMETERS

    mixed arg - The arguments to assemble into a callback.

### RETURNS

    mixed[] - The assembled callback.

## DESCRIPTION

Assembles a callback function from the provided arguments.
This function is used to create a callable structure that can be
invoked later. The callback can be either a method on an object or
a function. The assembled callback is returned as an array.
Usage:
- When you need to create a callback for an object method:
`assemble_call_back(object, "method", args...)`
- When you need to create a callback for a function:
`assemble_call_back(function, args...)`
The function performs the following steps:
1. Checks if the provided arguments form a valid array.
2. Determines the size of the arguments array.
3. Checks if the first argument is an object. If so, it verifies that
the second argument is a valid method name on the object.
4. If the first argument is a function, it creates a callback with the
function and any additional arguments.
5. Returns the assembled callback as an array.

## call_back

## SYNOPSIS

    mixed call_back(mixed cb, mixed new_arg...)

### PARAMETERS

    mixed cb - The callback to execute.
    mixed new_arg - The arguments to pass to the callback.

### RETURNS

    mixed - The result of the callback execution.

## DESCRIPTION

Executes a callback with the given arguments.

## valid_function

## SYNOPSIS

    int valid_function(mixed f)

### PARAMETERS

    mixed f - The function to check.

### RETURNS

    int - 1 if the function is valid, otherwise 0.

## DESCRIPTION

Checks if a given function is valid and not owned by a destructed
object.

## consolidate

## SYNOPSIS

    string consolidate(int x, string str)

### PARAMETERS

    int x - The quantity of items.
    string str - The description of the item(s).

### RETURNS

    string - The consolidated string.

## DESCRIPTION

Returns a consolidated string for a given quantity and item
description, handling pluralization and special cases.

## ordinal

## SYNOPSIS

    string ordinal (int n)

### PARAMETERS

    int n - The integer to convert.

### RETURNS

    string - The ordinal string representation of the integer.

## DESCRIPTION

Converts an integer to its ordinal string representation.

## int_string

## SYNOPSIS

    string int_string (int num)

### PARAMETERS

    int num - The numerical number to convert.

### RETURNS

    string - The worded number representation of the integer.

## DESCRIPTION

Converts a numerical number to its worded number representation.

## identify

## SYNOPSIS

    varargs string identify( mixed a, string indent )

### PARAMETERS

    mixed a - The variable to identify.
    string [indent] - The indentation string to use for formatting.

### RETURNS

    string - The string representation of the variable.

## DESCRIPTION

Converts a variable to its string representation for debugging
purposes, handling various data types and preventing deep
recursion.

## json_decode

## SYNOPSIS

    mixed json_decode(string text)

### PARAMETERS

    string text - The JSON string to deserialize.

### RETURNS

    mixed - The deserialized LPC value.

## DESCRIPTION

Deserializes a JSON string into an LPC value.

## json_encode

## SYNOPSIS

    varargs string json_encode(mixed value, mixed* pointers)

### PARAMETERS

    mixed value - The LPC value to serialize.
    mixed[] [pointers] - An optional array of pointers to handle circular references.

### RETURNS

    string - The JSON string representation of the LPC value.

## DESCRIPTION

Serializes an LPC value into a JSON string.

## element_of_weighted

## SYNOPSIS

    mixed element_of_weighted(mapping m)

### PARAMETERS

    mapping m - The weighted mapping to select from, where keys are the elements and values are their weights.

### RETURNS

    mixed - The selected element.

## DESCRIPTION

Selects an element from a weighted mapping based on their weights.

## pretty_map

## SYNOPSIS

    string pretty_map(mapping map)

### PARAMETERS

    mapping map - The mapping to format.

### RETURNS

    string - The formatted string representation of the mapping.

## DESCRIPTION

Returns a formatted string representation of a mapping, removing
any size annotations.

## tell_all

## SYNOPSIS

    varargs void tell_all(object ob, string str, int msg_type, mixed exclude)

### PARAMETERS

    object ob - The object to send the message from.
    string str - The message string to send.
    int [msg_type] - The message type, combined with ALL_MSG.
    mixed [exclude] - The objects to exclude from receiving the message.

## DESCRIPTION

Sends a message to all objects in the same environment, regardless
of their position in the containment hierarchy.

## tell

## SYNOPSIS

    varargs void tell(object ob, string str, int msg_type)

### PARAMETERS

    object ob - The object to send the message to.
    string str - The message string to send.
    int [msg_type] - The message type.

## DESCRIPTION

Sends a direct message to the specified object without considering
containment hierarchy.

## tell_down

## SYNOPSIS

    varargs void tell_down(object ob, string str, int msg_type, mixed exclude)

### PARAMETERS

    object ob - The object to send the message from.
    string str - The message string to send.
    int [msg_type] - The message type, combined with DOWN_MSG.
    mixed [exclude] - The objects to exclude from receiving the message.

## DESCRIPTION

Sends a message downward through the containment hierarchy, such
as from a container to all objects it contains.

## tell_up

## SYNOPSIS

    varargs void tell_up(object ob, string str, int msg_type, mixed exclude)

### PARAMETERS

    object ob - The object to send the message from.
    string str - The message string to send.
    int [msg_type] - The message type, combined with UP_MSG.
    mixed [exclude] - The objects to exclude from receiving the message.

## DESCRIPTION

Sends a message upward through the containment hierarchy, such as
from an object to its container, and further up to the room or
environment.

## tell_direct

## SYNOPSIS

    varargs void tell_direct(object ob, string str, int msg_type)

### PARAMETERS

    object ob - The object to send the message to.
    string str - The message string to send.
    int [msg_type] - The message type, combined with DIRECT_MSG.

## DESCRIPTION

Sends a direct message to the specified object without considering
containment hierarchy.

## percent_of

## SYNOPSIS

    float percent_of(float a, float b)

### PARAMETERS

    float a - The part value.
    float b - The whole value.

### RETURNS

    float - The percentage of `a` in `b`.

## DESCRIPTION

Calculates what percentage `a` is of `b`.

## remainder

## SYNOPSIS

    varargs float remainder(mixed a, mixed b)

### PARAMETERS

    mixed a - The dividend.
    mixed b - The divisor.

### RETURNS

    float - The remainder of `a` divided by `b`.

## DESCRIPTION

Calculates the remainder of `a` divided by `b`.

## percent

## SYNOPSIS

    float percent(float a, float b)

### PARAMETERS

    float a - The part value.
    float b - The whole value.

### RETURNS

    float - The percentage of `a` out of `b`.

## DESCRIPTION

Calculates the percentage of `a` out of `b`.

## range

## SYNOPSIS

    float range(float min, float max, float val)

### PARAMETERS

    float min - The minimum value.
    float max - The maximum value.
    float val - The value to check.

### RETURNS

    float - The value, constrained within the range of `min` to `max`.

## DESCRIPTION

Ensures a value is within a specified range.

## sum

## SYNOPSIS

    int sum(mixed *arr)

### PARAMETERS

    mixed[] arr - The array of numbers to sum.

### RETURNS

    int - The sum of all elements in the array.

## DESCRIPTION

Calculates the sum of all elements in an array.

## get_objects

## SYNOPSIS

    varargs mixed get_objects( string str, object player, int no_arr )

### PARAMETERS

    string str - The search string specifying the objects to locate.
    object [player] - The player object to use as a reference for searching.
    int [no_arr] - If specified, only a single object or 0 will be returned, otherwise an array of objects may be returned.

### RETURNS

    mixed - The located object(s), or 0 if not found.

## DESCRIPTION

Locates objects based on the specified search string, which can
include various search criteria and options.

## top_environment

## SYNOPSIS

    object top_environment(object ob)

### PARAMETERS

    object ob - The object to get the top-level environment of.

### RETURNS

    object - The top-level environment of the object.

## DESCRIPTION

Retrieves the top-level environment of the specified object,
traversing up through nested environments.

## find_ob

## SYNOPSIS

    varargs object find_ob(mixed ob, mixed cont, function f)

### PARAMETERS

    mixed ob - The object or name of the object to find.
    mixed [cont] - The container or environment to search within. Defaults to the previous object.
    function [f] - An optional function to further filter the search.

### RETURNS

    object - The found object, or 0 if not found.

## DESCRIPTION

Searches for an object within a container or environment
using the specified criteria.

## getoid

## SYNOPSIS

    int getoid(object ob)

### PARAMETERS

    object ob - The object to get the ID of.

### RETURNS

    int - The unique object ID.

## DESCRIPTION

Retrieves the unique object ID of the given object.

## get_object

## SYNOPSIS

    varargs object get_object( string str, object player )

### PARAMETERS

    string str - The name of the object to locate.
    object [player] - The player object to use as a reference for searching.

### RETURNS

    object - The located object, or 0 if not found.

## DESCRIPTION

Attempts to locate an object by the given name and returns the
object pointer if found.

## resolve_path

## SYNOPSIS

    string resolve_path(string Current, string Next)

### PARAMETERS

    string Current - The current path.
    string Next - The next path to resolve.

### RETURNS

    string - The resolved absolute path.

## DESCRIPTION

Resolves a given path relative to the current path, handling
special cases such as user directories and relative paths.

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

## devp

## SYNOPSIS

    varargs int devp(mixed user)

### PARAMETERS

    mixed user - The user to check, either as a username string or an object. Defaults to the previous object.

### RETURNS

    int - 1 if the user has developer privileges, otherwise 0.

## DESCRIPTION

Checks if a user has developer privileges.

## adminp

## SYNOPSIS

    int adminp(mixed user)

### PARAMETERS

    mixed user - The user to check, either as a username string or an object. Defaults to the previous object.

### RETURNS

    int - 1 if the user has admin privileges, otherwise 0.

## DESCRIPTION

Checks if a user has admin privileges.

## wizardp

## SYNOPSIS

    int wizardp(mixed user) { return devp(user); }

### PARAMETERS

    mixed user - The user to check, either as a username string or an object.

### RETURNS

    int - 1 if the user has developer privileges, otherwise 0.

## DESCRIPTION

Checks if a user has developer privileges (alias for devp).

## is_member

## SYNOPSIS

    int is_member(string user, string group)

### PARAMETERS

    string user - The username to check.
    string group - The group to check membership in.

### RETURNS

    int - 1 if the user is a member of the group, otherwise 0.

## DESCRIPTION

Checks if a user is a member of a specified group.

## dump_socket_status

## SYNOPSIS

    string dump_socket_status()

### RETURNS

    string - The formatted socket status information.

## DESCRIPTION

Returns a formatted string displaying the status of all sockets.

## substr

## SYNOPSIS

    varargs string substr(string str, string sub, int reverse)

### PARAMETERS

    string str - The string to extract from.
    string sub - The substring to extract to.
    int [reverse=0] - If set, the substring will start at the last occurrence.

### RETURNS

    string - The extracted substring.

## DESCRIPTION

Returns a substring of a string, starting from 0 and ending at the
first occurrence of another string within it. If the reverse flag
is set, the substring will start at the last occurrence of the
substring within the string.

## extract

## SYNOPSIS

    varargs string extract(string str, int from, int to)

### PARAMETERS

    string str - The string to extract from.
    int from - The starting position to extract from.
    int [to] - The ending position to extract to.

### RETURNS

    string - The extracted substring.

## DESCRIPTION

Extracts a substring from a string.

## reverse_strsrch

## SYNOPSIS

    varargs int reverse_strsrch(string str, string sub, int start)

### PARAMETERS

    string str - The string to search in.
    string sub - The substring to search for.
    int [start=-1] - The starting position to search from.

### RETURNS

    int - The position of the substring in the string, or -1 if not found.

## DESCRIPTION

Searches for a substring in a string starting from a given position
and moving backwards.

## append

## SYNOPSIS

    string append(string source, string to_append)

### PARAMETERS

    string source - The string to append to.
    string to_append - The string to append.

### RETURNS

    string - The original string with the appended string if it was not already present.

## DESCRIPTION

Appends a string to another string if it is not already there.
If the string is already present, the original string is returned.

## no_ansi

## SYNOPSIS

    string no_ansi(string str)

### PARAMETERS

    string str - The string to remove ANSI codes from.

### RETURNS

    string - The string without ANSI codes.

## DESCRIPTION

Returns a string with all ANSI colour codes removed.

## chop

## SYNOPSIS

    varargs string chop(string str, string sub, int dir)

### PARAMETERS

    string str - The string to chop from.
    string sub - The substring to chop.
    int [dir=-1] - The direction to chop: 1 for left, -1 for right.

### RETURNS

    string - The string with the substring chopped off if it was present.

## DESCRIPTION

Chops a substring off the end or beginning of another string if
it is present. If the substring is not present, the original
string is returned.

## from_string

## SYNOPSIS

    varargs mixed from_string(string str, int flag)

### PARAMETERS

    string str - The string to convert.
    int [flag=0] - If set, returns an array with the value and the remaining string.

### RETURNS

    mixed - The LPC value represented by the string.

## DESCRIPTION

Converts a string representation of an LPC value to the corresponding
LPC value.

## reverse_string

## SYNOPSIS

    string reverse_string(string str)

### PARAMETERS

    string str - The string to reverse.

### RETURNS

    string - The reversed string.

## DESCRIPTION

Reverses a string.

## simple_list

## SYNOPSIS

    varargs string simple_list(string *arr, string conjunction)

### PARAMETERS

    string[] arr - The array to make a list from.
    string [conjunction="and"] - The word to join the last two elements of the list.

### RETURNS

    string - The simple list string.

## DESCRIPTION

Returns a string that is a simple list of the elements of an array,
joined by a conjunction.

## add_commas

## SYNOPSIS

    string add_commas(mixed number)

### PARAMETERS

    mixed number - The number to add commas to.

### RETURNS

    string - The number with commas added as a string.

## DESCRIPTION

Returns a string with commas added to the number.

## stringify

## SYNOPSIS

    string stringify(mixed val)

### PARAMETERS

    mixed val - The value to convert.

### RETURNS

    string - The string representation of the value.

## DESCRIPTION

Converts an LPC value to its string representation.

## prepend

## SYNOPSIS

    string prepend(string source, string to_prepend)

### PARAMETERS

    string source - The string to prepend to.
    string to_prepend - The string to prepend.

### RETURNS

    string - The original string with the prepended string if it was not already present.

## DESCRIPTION

Prepends a string to another string if it is not already there.
If the string is already present, the original string is returned.

## mud_config

## SYNOPSIS

    mixed mud_config(string str)

### PARAMETERS

    string str - The configuration key to retrieve.

### RETURNS

    mixed - The configuration value.

## DESCRIPTION

Retrieves a specific configuration value from the MUD config.

## mud_name

## SYNOPSIS

    string mud_name()

### RETURNS

    string - The name of the MUD.

## DESCRIPTION

Returns the name of the MUD.

## _ok

## SYNOPSIS

    int _ok(mixed str, mixed args...)

### PARAMETERS

    string str - The informational message.
    mixed [args] - Optional arguments to format the message.

### RETURNS

    int - Always returns 1, unless there is no previous object.

## DESCRIPTION

Provides an informational message, optionally formatted with arguments.

## lib_name

## SYNOPSIS

    string lib_name()

### RETURNS

    string - The library name.

## DESCRIPTION

Returns the name of the library being used by the MUD.

## tmp_dir

## SYNOPSIS

    string tmp_dir()

### RETURNS

    string - The temporary directory.

## DESCRIPTION

Returns the directory where temporary files are stored.

## admin_email

## SYNOPSIS

    string admin_email()

### RETURNS

    string - The admin email address.

## DESCRIPTION

Returns the admin email address for the MUD.

## arch

## SYNOPSIS

    string arch()

### RETURNS

    string - The system architecture.

## DESCRIPTION

Returns the architecture of the system the MUD is running on.

## port

## SYNOPSIS

    int port()

### RETURNS

    int - The port number.

## DESCRIPTION

Returns the port number the MUD is running on.

## baselib_name

## SYNOPSIS

    string baselib_name()

### RETURNS

    string - The base library name.

## DESCRIPTION

Returns the name of the base library.

## driver_version

## SYNOPSIS

    string driver_version()

### RETURNS

    string - The driver version.

## DESCRIPTION

Returns the version of the MUD driver.

## lib_version

## SYNOPSIS

    string lib_version()

### RETURNS

    string - The library version.

## DESCRIPTION

Returns the version of the library being used by the MUD.

## baselib_version

## SYNOPSIS

    string baselib_version()

### RETURNS

    string - The base library version.

## DESCRIPTION

Returns the version of the base library.

## _warn

## SYNOPSIS

    int _warn(mixed str, mixed args...)

### PARAMETERS

    string str - The warning message.
    mixed [args] - Optional arguments to format the message.

### RETURNS

    int - Always returns 1, unless there is no previous object.

## DESCRIPTION

Provides a warning message, optionally formatted with arguments.

## _error

## SYNOPSIS

    int _error(mixed str, mixed args...)

### PARAMETERS

    string str - The error message.
    mixed [args] - Optional arguments to format the message.

### RETURNS

    int - Always returns 1, unless there is no previous object.

## DESCRIPTION

Provides an error message, optionally formatted with arguments.

## open_status

## SYNOPSIS

    string open_status()

### RETURNS

    string - The open status of the MUD.

## DESCRIPTION

Returns the open status of the MUD.

## log_dir

## SYNOPSIS

    string log_dir()

### RETURNS

    string - The log directory.

## DESCRIPTION

Returns the directory where log files are stored.

