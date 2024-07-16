## shout

## SYNOPSIS

```c
varargs void shout(string msg, mixed exclude)
```

### PARAMETERS

* `string msg` - The message to write.
* `mixed exclude` - The object or array of objects to exclude from the message.

## DESCRIPTION

This function is an override of the shout() efun. It will
write a message to all users using the message() efun.

## set_privs

## SYNOPSIS

```c
void set_privs(object ob, string privs)
```

### PARAMETERS

* `object ob` - The object to destruct.

## DESCRIPTION

This function is an override for the efun destruct(). It checks
if the object calling the function has admin privs. If it does,
it will execute the function as normal. If it does not, it will
return 0.

## ctime

## SYNOPSIS

```c
varargs string ctime(int x)
```

### PARAMETERS

* `int x` - The time to convert to a string.

## DESCRIPTION

This function is an override of the efun ctime(). It will
return the current time in the format "YYYY-MM-DD HH:MM:SS".
It also will use time() if no argument is provided.

## tell_object

## SYNOPSIS

```c
varargs void tell_object(mixed ob, mixed msg, mixed type)
```

### PARAMETERS

* `object ob` - The object to write the message to.
* `string msg` - The message to write.
* `string type` - The message class of message to write.

## DESCRIPTION

This function is an override of the tell_object() efun. It will
write a message to the object using the message() efun.

## write

## SYNOPSIS

```c
void write(string msg)
```

### PARAMETERS

* `string msg` - The message to write.

## DESCRIPTION

This function is an override of the write() efun. It will
write a message to the player using the message() efun
if this_player(), otherwise it will use the debug_message()
efun.

## this_body

## SYNOPSIS

```c
object this_body()
```

## DESCRIPTION

This is an override of the efun this_body(). It will return
the this_player().

## query_num

## SYNOPSIS

```c
varargs string query_num(int x, int many)
```

### PARAMETERS

* `int x` - The number to convert to a string.
* `int many` - If true, the number will be returned in plural form.

## DESCRIPTION

This function is an override of query_num(). It will return
the number in string form.

## say

## SYNOPSIS

```c
varargs void say(string msg, mixed exclude)
```

### PARAMETERS

* `string msg` - The message to write.
* `mixed exclude` - The object or array of objects to exclude from the message.

## DESCRIPTION

This function is an override of the say() efun. It will
write a message to the room using the message() efun.

## shutdown

## SYNOPSIS

```c
void shutdown( int how )
```

### PARAMETERS

* `int how` - The type of shutdown to perform.

## DESCRIPTION

This function is an override for the efun shutdown(). It checks
if the object calling the function has admin privs. If it does,
it will execute the function as normal. If it does not, it will
return 0.

## tell_room

## SYNOPSIS

```c
varargs void tell_room(mixed room, string msg, mixed exclude)
```

### PARAMETERS

* `mixed room` - The room to write the message to.
* `string msg` - The message to write.
* `mixed exclude` - The object or array of objects to exclude from the message.

## DESCRIPTION

This function is an override of the tell_room() efun. It will
write a message to the room using the message() efun.

## exec

## SYNOPSIS

```c
int exec( object to, object from )
```

### PARAMETERS

* `object to` - The object to exec to.
* `object from` - The object to exec from.

## DESCRIPTION

This function is an override for the efun exec(). It checks
if the object calling the function has admin privs. If it does,
it will
execute the function as normal. If it does not, it will return 0.

## userp

## SYNOPSIS

```c
varargs int userp(object ob)
```

### PARAMETERS

* `object ob` - The object to check.

## DESCRIPTION

This function is an override of the userp() efun. Unlike the
efun, this function accepts a null argument and will check
if the previous object is a user if no argument is provided.

## this_user

## SYNOPSIS

```c
object this_user()
```

## DESCRIPTION

This is an override of the efun this_user(). It will return
the this_player()->query_user().

