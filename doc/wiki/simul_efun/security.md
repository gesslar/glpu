## devp

## SYNOPSIS

```c
varargs int devp(mixed user)
```

### PARAMETERS

* `mixed user` - The user to check, either as a username string or an object. Defaults to the previous object.

## DESCRIPTION

Checks if a user has developer privileges.

## adminp

## SYNOPSIS

```c
int adminp(mixed user)
```

### PARAMETERS

* `mixed user` - The user to check, either as a username string or an object. Defaults to the previous object.

## DESCRIPTION

Checks if a user has admin privileges.

## wizardp

## SYNOPSIS

```c
int wizardp(mixed user) { return devp(user); }
```

### PARAMETERS

* `mixed user` - The user to check, either as a username string or an object.

## DESCRIPTION

Checks if a user has developer privileges (alias for devp).

## is_member

## SYNOPSIS

```c
int is_member(string user, string group)
```

### PARAMETERS

* `string user` - The username to check.
* `string group` - The group to check membership in.

## DESCRIPTION

Checks if a user is a member of a specified group.

