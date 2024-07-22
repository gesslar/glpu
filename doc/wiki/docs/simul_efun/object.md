---
title: object
---
# object.c

## get_objects

### Synopsis

```c
varargs mixed get_objects( string str, object player, int no_arr )
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

### Description

Locates living objects by the specified names within the
specified room.

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

### Description

Locates a living object by the specified name within the
specified room.

## get_object

### Synopsis

```c
varargs object get_object( string str, object player )
```

### Parameters

* `string str` - The name of the object to locate.
* `object [player]` - The player object to use as a reference for searching.

### Returns

`object` - The located object, or 0 if not found.

### Description

Attempts to locate an object by the given name and returns the
object pointer if found.

