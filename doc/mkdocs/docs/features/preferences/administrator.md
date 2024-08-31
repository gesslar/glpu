# Administrator Preferences

All of the preferences listed in the [User Preferences](user.md) section apply
to administrators as well. In addition, the following preferences are available
to administrators:

To set a preference, use the `set` command. For example:

```
set log_level 4
```

To clear a preference, set the preference with no value. For example:

```
set log_level
```

## custom_clone

This variable allows you to provide a custom message to be printed whenever you
clone an object. `$N` will be replaced with your name, and `$O` will be replaced
with the name of the object.

## custom_dest

This variable allows you to provide a custom message to be printed whenever you
destruct an object. `$N` will be replaced with your name, and `$O` will be
replaced with the name of the object.

## feedback_level

When you receive system messages of `ok`, `inform`, `warn`, or `error`, this
variable allows you to set the tagging that appears before the message.

If this is not set, then you will receive the default system tags for feedback
messages.

* `normal` - a coloured symbol, attempting to use Unicode
* `plain` - a coloured symbol, using ASCII
* `none` - no symbol or file tag

If you are using a screenreader, then you will receive feedback decoration.

## log_level

This variable allows you to set the level of logging that body and user events
will generate. This can be set on players, but as this information is printed
to the `debug.log`, they won't see it. This is useful for debugging purposes.

REMEMBER: This will generate a lot of information, so be careful with this and
don't forget to turn it off.

Levels are `1` to `4`, with `1` being the least nerdy information and `4` being
the most nerdy information. `0` or unset will turn off logging.

## look_filename

This variable allows you to set whether you see the file names of the room
or objects when you look at them. The default is `on`.

* `on` - show for rooms only
* `off` - do not show for rooms or objects
* `all` - show for rooms and objects

## move_in

This variable allows you to provide a custom message for whenever you move
normally to a room. The `move_in` message will be printed to the room the you
move into.

`$N` will be replaced with your name, and `$D` will be replaced with the
direction/exit you use.

*Note: You are not required to use either `$N` or `$D` you may put your name in
literally if you wish.*

## move_out

This variable allows you to provide a custom message for whenever you move
normally from a room. The `move_out` message will be printed to the room you
move from.

`$N` will be replaced with your name, and `$D` will be replaced with the

*Note: You are not required to use either `$N` or `$D` you may put your name in
literally if you wish.*

## start_location

This variable allows you to set a start_location besides the standard one
You must provide a full path to a room or set it to `last_location` to return
to the last room you were in before logging out.

If you do not set a start_location, you will default to your workroom in your
home directory if it exists. Otherwise, you will login to the same start
location as players.

## teleport_in

This variable allows you to provide custom messages for whenever you teleport
to a different place. The `teleport_in` message is printed to the room the you
teleport to.

`$N` in the message will be replaced with your name, and $D will be replaced
with the location being teleported to.

*Note: You are not required to use either `$N` or `$D` you may put your name in
literally if you wish.*

## teleport_out

This variable allows you to provide custom messages for whenever you teleport
from a room. The `teleport_out` message is printed to the room you teleport
from.

`$N` in the message will be replaced with your name, and $D will be replaced
with the location being teleported from.

*Note: You are not required to use either `$N` or `$D` you may put your name in
literally if you wish.*
