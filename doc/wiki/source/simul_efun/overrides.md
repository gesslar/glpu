# ctime

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


# exec

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


# query_num

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


# say

## SYNOPSIS

    varargs void say(string msg, mixed exclude)

### PARAMETERS

    string msg - The message to write.
    mixed exclude - The object or array of objects to exclude from the message.

### RETURNS


## DESCRIPTION

This function is an override of the say() efun. It will
write a message to the room using the message() efun.


# set_privs

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


# shout

## SYNOPSIS

    varargs void shout(string msg, mixed exclude)

### PARAMETERS

    string msg - The message to write.
    mixed exclude - The object or array of objects to exclude from the message.

### RETURNS


## DESCRIPTION

This function is an override of the shout() efun. It will
write a message to all users using the message() efun.


# shutdown

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


# tell_object

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


# tell_room

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


# this_body

## SYNOPSIS

    object this_body()

### RETURNS

    object - The this_player().

## DESCRIPTION

This is an override of the efun this_body(). It will return
the this_player().


# this_user

## SYNOPSIS

    object this_user()

### RETURNS

    object - The this_player()->query_user().

## DESCRIPTION

This is an override of the efun this_user(). It will return
the this_player()->query_user().


# userp

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


# write

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


