---
title: security
---
# security.c

## devp

### Synopsis

```c
varargs int devp(mixed user)
```

### Parameters

* `mixed user` - The user to check, either as a username string or an object. Defaults to the previous object.

### Returns

`int` - 1 if the user has developer privileges, otherwise 0.

### Description

Checks if a user has developer privileges.

## adminp

### Synopsis

```c
int adminp(mixed user)
```

### Parameters

* `mixed user` - The user to check, either as a username string or an object. Defaults to the previous object.

### Returns

`int` - 1 if the user has admin privileges, otherwise 0.

### Description

Checks if a user has admin privileges.

## wizardp

### Synopsis

```c
int wizardp(mixed user) { return devp(user); }
```

### Parameters

* `mixed user` - The user to check, either as a username string or an object.

### Returns

`int` - 1 if the user has developer privileges, otherwise 0.

### Description

Checks if a user has developer privileges (alias for devp).

## is_member

### Synopsis

```c
int is_member(string user, string group)
```

### Parameters

* `string user` - The username to check.
* `string group` - The group to check membership in.

### Returns

`int` - 1 if the user is a member of the group, otherwise 0.

### Description

Checks if a user is a member of a specified group.

