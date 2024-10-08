---
title: overrides
---
# overrides.c

## shout

### Synopsis

```c
varargs void shout(string msg, mixed exclude)
```

### Parameters

* `string msg` - The message to write.
* `mixed exclude` - The object or array of objects to exclude from the message.

### Returns

{void}

### Description

This function is an override of the shout() efun. It will
write a message to all users using the message() efun.

## set_privs

### Synopsis

```c
void set_privs(object ob, string privs)
```

### Parameters

* `object ob` - The object to destruct.

### Returns

{void}

### Description

This function is an override for the efun destruct(). It checks
if the object calling the function has admin privs. If it does,
it will execute the function as normal. If it does not, it will
return 0.

## ctime

### Synopsis

```c
varargs string ctime(int x)
```

### Parameters

* `int x` - The time to convert to a string.

### Returns

`string` - The time in string form.

### Description

This function is an override of the efun ctime(). It will
return the current time in the format "YYYY-MM-DD HH:MM:SS".
It also will use time() if no argument is provided.

## tell_object

### Synopsis

```c
varargs void tell_object(mixed ob, mixed msg, mixed type)
```

### Parameters

* `object ob` - The object to write the message to.
* `string msg` - The message to write.
* `string type` - The message class of message to write.

### Returns

{void}

### Description

This function is an override of the tell_object() efun. It will
write a message to the object using the message() efun.

## write

### Synopsis

```c
void write(string msg)
```

### Parameters

* `string msg` - The message to write.

### Returns

{void}

### Description

This function is an override of the write() efun. It will
write a message to the player using the message() efun
if this_body(), otherwise it will use the debug_message()
efun.

## query_num

### Synopsis

```c
varargs string query_num(int x, int many)
```

### Parameters

* `int x` - The number to convert to a string.
* `int many` - If true, the number will be returned in plural form.

### Returns

`string` - The number in string form.

### Description

This function is an override of query_num(). It will return
the number in string form.

## say

### Synopsis

```c
varargs void say(string msg, mixed exclude)
```

### Parameters

* `string msg` - The message to write.
* `mixed exclude` - The object or array of objects to exclude from the message.

### Returns

{void}

### Description

This function is an override of the say() efun. It will
write a message to the room using the message() efun.

## shutdown

### Synopsis

```c
void shutdown(int how)
```

### Parameters

* `int how` - The type of shutdown to perform.

### Returns

{void}

### Description

This function is an override for the efun shutdown(). It checks
if the object calling the function has admin privs. If it does,
it will execute the function as normal. If it does not, it will
return 0.

## tell_room

### Synopsis

```c
varargs void tell_room(mixed room, string msg, mixed exclude)
```

### Parameters

* `mixed room` - The room to write the message to.
* `string msg` - The message to write.
* `mixed exclude` - The object or array of objects to exclude from the message.

### Returns

{void}

### Description

This function is an override of the tell_room() efun. It will
write a message to the room using the message() efun.

## exec

### Synopsis

```c
int exec(object to, object from)
```

### Parameters

* `object to` - The object to exec to.
* `object from` - The object to exec from.

### Returns

`int` - 1 if the exec was successful, 0 if it was not.

### Description

This function is an override for the efun exec(). It checks
if the object calling the function has admin privs. If it does,
it will execute the function as normal. If it does not, it will
return 0.

## userp

### Synopsis

```c
varargs int userp(object ob)
```

### Parameters

* `object ob` - The object to check.

### Returns

`int` - 1 if the object is a user, otherwise 0.

### Description

This function is an override of the userp() efun. Unlike the
efun, this function accepts a null argument and will check
if the previous object is a user if no argument is provided.

