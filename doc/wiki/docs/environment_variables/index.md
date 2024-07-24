# Environment Variables

The following categories of environment variables apply to whether you are
an [administrator](administrator-environment-variables) or a player.

Generally, if an environment variable offers `on` or `off` as options, the
default is `off`, unless otherwise noted.

To set an environment variable, use the `set` command. For example:

```
set colour on
```

To clear an environment variable, set the variable with no value. For example:

```
set colour
```

## auto_tune

This variable allows you to specify which channels or channels you wish to
automatically tune into when you login. If you wish to tune into multiple
channels or modules, separate the names by spaces, or `all` to tune into all
available channels or modules.

To see a list of channels, type `channel list all`.

## biff

This variable will set whether or not you recieve the new mail notification
when you log in, or when someone sends you a piece of mail. The default is
`on`.

* `on` - Enable the new mail notification.
* `off` - Disable the new mail notification.

## colour

Determines whether colour is enabled and will be sent to the terminal.

* `on` - Enable colour.
* `off` - Disable colour.

Use the `colour` command to set this variable.

## gmcp

Determines whether GMCP is enabled. The default is `off`. May also be managed
by the `gmcp` command.

* `on` - Enable GMCP.
* `off` - Disable GMCP.

## highlight

Determines whether items of interest are highlighted when you look in a room
or at an item. The default for the distribution is `on`.

* `on` - Enable highlighting.
* `off` - Disable highlighting.

## highlight_color

Determines the colour used for highlighting. The default for the distribution
is `243`. Use the `colour list` command for a list of available colours.

## keepalive

Determines whether the game will send keepalive packets to the client. The
default is `off`.

* `on` - Enable keepalive packets.
* `off` - Disable keepalive packets.

## morelines

Determines the number of lines to display before pausing when the `more`
prompt is displayed. The default is `40`.

## unicode

Determines whether Unicode characters are used in the client. The default is
`off`.

* `on` - Enable Unicode.
* `off` - Disable Unicode.

## page_display

This variable allows you to change whether you see the percentage of a document
viewed or the line numbers of the current page of a document. The default is
`line`.

* `percent` - Display the percentage of the document viewed.
* `line` - Display the line numbers of the current page.

## screenreader

Determines whether the client is in screen reader mode. The default is `off`.
However, if you connect with a client that the game detects has screenreading
capabilities, this will be overridden.

* `on` - Enable screen reader mode.
* `off` - Disable screen reader mode.
