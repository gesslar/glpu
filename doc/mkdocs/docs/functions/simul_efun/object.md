---
title: object
---
# object.c

## same_env_check

### Synopsis

```c
varargs int same_env_check(object one, object two, int top_env)
```

### Parameters

* `object one` - The first object to compare.
* `object two` - The second object to compare.
* `int top_env` - Whether to check the top-level environment.

### Returns

`int` - 1 if the objects are in the same environment, 0 otherwise.

### Description

This simul_efun will return 1 if the two objects are in the
same environment, or 0 if they are not.

If top_env is set to 1, the simul_efun will check if the two
objects share the same top-level environment.

## caller_is

### Synopsis

```c
int caller_is(mixed ob)
```

### Parameters

* `mixed ob` - The object or file name to compare the caller to.

### Returns

`int` - 1 if the caller is the specified object, 0 otherwise.

### Description

This simul_efun will return 1 if the caller of the current
operation is the specified object, or if the caller is the
object with the specified file name.

*NOTE* This simul_efun will not succeed when called from
other functions in the simul_efun object, as previous_object()
does not count the object itself in its call list.

## all_environment

### Synopsis

```c
object *all_environment(object ob)
```

### Parameters

* `object ob` - The object to get the environments of.

### Returns

`object*` - An array of environments of the object.

### Description

Retrieves all environments of the specified object, traversing
up through nested environments.

## get_objects

### Synopsis

```c
varargs mixed get_objects(string str, object player, int no_arr)
```

### Parameters

* `string str` - The search string specifying the objects to locate.
* `object [player]` - The player object to use as a reference for searching.
* `int [no_arr]` - If specified, only a single object or 0 will be returned, otherwise an array of objects may be returned.

### Returns

`mixed` - The located object(s), or 0 if not found.

### Description

Locates objects based on the specified search string, which can
include various search criteria and options.

## present_players

### Synopsis

```c
object *present_players(object room)
```

### Parameters

* `object room` - The room to search for player objects in.

### Returns

`object*` - An array of player objects present in the room.

### Description

Retrieves all player objects present in the specified room.

## top_environment

### Synopsis

```c
object top_environment(object ob)
```

### Parameters

* `object ob` - The object to get the top-level environment of.

### Returns

`object` - The top-level environment of the object.

### Description

Retrieves the top-level environment of the specified object,
traversing up through nested environments.

## get_players

### Synopsis

```c
object *get_players(mixed names, object room)
```

### Parameters

* `string|string* names` - The name of the player objects to locate.
* `object room` - The room to search for the player objects in.

### Returns

`object*` - An array of located player objects.

### Description

Locates player objects by the specified names within the

## find_ob

### Synopsis

```c
varargs object find_ob(mixed ob, mixed cont, function f)
```

### Parameters

* `mixed ob` - The object or name of the object to find.
* `mixed [cont]` - The container or environment to search within. Defaults to the previous object.
* `function [f]` - An optional function to further filter the search.

### Returns

`object` - The found object, or 0 if not found.

### Description

Searches for an object within a container or environment
using the specified criteria.

## this_body

### Synopsis

```c
object this_body()
```

### Returns

`object` - The body of the current calling player.

### Description

This is a simul_efun that will return the body of the current
interactive user. It is used as a replacement for this_player().

## clones

### Synopsis

```c
varargs object *clones(mixed file, int env_only)
```

### Parameters

* `mixed file` - The file or object to find clones of.
* `int env_only` - Whether to only return clones that have an environment.

### Returns

`object*` - An array of objects that are clones of the specified file.

### Description

Retrieves all clones of the specified file in the game. If
the file is an object, it will retrieve all clones using the
object's base name. If env_only is set to 1, the simul_efun
will only return clones that have an environment.

## present_clones

### Synopsis

```c
object *present_clones(mixed file, object container)
```

### Parameters

{mixed} file
{object} container

### Returns

`object*` - An array of clones of the specified file present in the container.

### Description

Retrieves all clones of the specified file present in the
specified container. If the file is an object, it will
retrieve all clones using the object's base name.

## present_npcs

### Synopsis

```c
object *present_npcs(object room)
```

### Parameters

* `object room` - The room to search for NPCs in.

### Returns

`object*` - An array of NPC objects present in the room.

### Description

Retrieves all non-player characters (NPCs) present in the
specified room.

## present_livings

### Synopsis

```c
object *present_livings(object room)
```

### Parameters

* `object room` - The room to search for living objects in.

### Returns

`object*` - An array of living objects present in the room.

### Description

Retrieves all living objects present in the specified room.

## get_player

### Synopsis

```c
object get_player(string name, object room)
```

### Parameters

* `string name` - The name of the player to locate.
* `object room` - The room to search for the player in.

### Returns

`object` - The located player object, or 0 if not found.

### Description

Locates a player object by the specified name within the
specified room.

## get_livings

### Synopsis

```c
object *get_livings(mixed names, object room)
```

### Parameters

* `string|string* names` - The name of the living objects to locate.
* `object room` - The room to search for the living objects in.

### Returns

`object*` - An array of located living objects.

### Description

Locates living objects by the specified names within the
specified room.

## find_target

### Synopsis

```c
varargs object find_target(object tp, string arg, object source, function f)
```

### Parameters

* `object tp` - The body object of the player or NPC searching
* `string arg` - The argument to match objects against
* `object source` - The object to search within, such as a room or container. If not provided, the environment of the calling object will be used.
* `function f` - An optional custom filter function to further filter

### Returns

`object` - The located object, or 0 if not found.

### Description

This simul_efun will find a single object in a container, such
as a room, chest, player's inventory, that matches the specified
argument. The argument can be a name, ID, or other identifier
that can be used to match objects.

The simul_efun will then filter the objects based on the
visibility of the calling object, and apply an additional
custom filter function if provided.

## this_caller

### Synopsis

```c
object this_caller()
```

### Returns

`object` - The object that called the current operation.

### Description

This is a simul_efun that will return the object that called
the current operation. This may be this_body(), but it may
also be a shadow another player who initiated the chain.
For example, a wizard using the force command.

Be careful with this one, you don't want to accidentally
perform operations on the wrong object.

## find_targets

### Synopsis

```c
varargs object *find_targets(object tp, string arg, object source, function f)
```

### Parameters

* `object tp` - The body object of the player or NPC searching
* `string arg` - The argument to match objects against
* `object source` - The object to search within, such as a room or container. If not provided, the environment of the calling object will be used.
* `function f` - An optional custom filter function to further filter the objects.

### Returns

`object*` - An array of located objects or 0 if none are found.

### Description

This simul_efun will find all objects in a container, such as
a room, chest, player's inventory, that match the specified
argument. The argument can be a name, ID, or other identifier
that can be used to match objects.

The simul_efun will then filter the objects based on the
visibility of the calling object, and apply an additional
custom filter function if provided.


## getoid

### Synopsis

```c
int getoid(object ob)
```

### Parameters

* `object ob` - The object to get the ID of.

### Returns

`int` - The unique object ID.

### Description

Retrieves the unique object ID of the given object.

## get_living

### Synopsis

```c
object get_living(string name, object room)
```

### Parameters

* `string name` - The name of the living object to locate.
* `object room` - The room to search for the living object in.

### Returns

`object` - The located living object, or 0 if not found.

### Description

Locates a living object by the specified name within the
specified room.

## get_object

### Synopsis

```c
varargs object get_object(string str, object player)
```

### Parameters

* `string str` - The name of the object to locate.
* `object [player]` - The player object to use as a reference for searching.

### Returns

`object` - The located object, or 0 if not found.

### Description

Attempts to locate an object by the given name and returns the
object pointer if found.

