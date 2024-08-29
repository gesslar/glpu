The following outlines the GMCP specification for {{ MUD_NAME }}, including what is sent
by the server and what is sent by the client, as well as expectations and
any default behaviour.

!!! info

    ##### Oubound Messages

    * All numeric values are ultimately sent as strings, i.e. `10` is sent as
      `"10"` and `1.5` is sent as `"1.5"`.

    * {{ MUD_NAME }}, as a FluffOS MUD, does not support boolean values and
      therefore all boolean values are sent as `"true"` or `"false"` strings.

    * {{ MUD_NAME }}, as a FluffOS MUD, does not support the concept of
      `null`, `nil`, or `undefined` and therefore all such values are sent
      as `0`.

    * Although the examples provided below may be shown on multiple lines,
      they sent as a single line JSON string.

    ##### Inbound Messages

    * Boolean values sent as native `true` or `false` values are received
      just fine and are automatically converted to the corresponding LPC
      values of `1` and `0` respectively.

    * Null values sent as native `null` values are received just fine and are
      automatically converted to the corresponding LPC value of `0`.

    * Although the examples provided below may be shown on multiple lines,
      they are expected to have been sent as a single line JSON string.

## Supported Packages and modules

* [Core](#core) - Core functionality
* [Char](#char) - Character information
* [Comm](#comm) - Communication and channels
* [Room](#room) - Room information

## Core

### Client

#### Core.Hello

Identifies the client to the server. Client and version are sent as JSON
object. This should be the first message sent by the client to the server.

!!! example

    ``` json
    Core.Hello { "client": "Mudlet", "version": "1.0" }
    ```

#### Core.Ping

Sent by the client to the server to send a `Core.Ping` in response. A payload
is optional, and if present is an integer representing the average ping
time from previous requests.

!!! example

    ``` json
    Core.Ping 12
    ```

!!! note ""

    {{ MUD_NAME }} by default does not make use of the information provided in this
    message's payload.

#### Core.Supports

This module is used to inform the server of the client's supported features.

##### Core.Supports.Set

Send the client's supported features to the server, all at once in one message.

The format a JSON array of strings and integers where the strings are the GMCP
packages and the integers are the module versions.

!!! example

    ``` json
    Core.Supports.Set [ "Core 1", "Char 2", "Comm 1", "Room 1" ]
    ```

!!! note ""

    Per the GMCP specification, `Core.Supports.Set` will always overwrite
    any existing information, whether from previous `.Set`, `.Add`, or
    `.Remove` messages.

!!! note ""

    {{ MUD_NAME }} does not make use of the module version information
    supplied by the client.




##### Core.Supports.Add

Ordinarily, all information from a client is sent at once via `Core.Supports.Set`,
however, sometimes there are plugins or extensions to a MUD client that add
additional capabilities. In such cases, `Core.Supports.Add` can be used to
announce those capabilities to the server. They are sent one at a time in a
similar format to `Core.Supports.Set` with the name of the package and the
version number.

!!! example

    ``` json
    Core.Supports.Add "Client 1"
    ```

##### Core.Supports.Remove

Remove any module from being supported. This informs {{ MUD_NAME }} that the
client would no longer like to receive information for that module.

The format is a JSON array with the name(s) of the module(s) to remove. No
version number is required.

!!! example

    ``` json
    Core.Supports.Remove [ "Client", "Room" ]
    ```

### Server

#### Core.Ping

Sent by the server to the client to respond to a `Core.Ping`. There is no
payload.

!!! example

    ``` json
    Core.Ping
    ```

#### Core.Goodbye

Sent by the server to the client to inform that the character has been logged
out. The payload is a string which can contain any message, including a
reason for the disconnection.

!!! example

    ``` json
    Core.Goodbye "Farewell, adventurer!"
    ```

## Char

### Client

#### Char.Login

This module handles the login process for a character.

##### Char.Login.Credentials

Sent by the client to the server to login a character. The payload is a JSON
object with the following keys:

* `account` - The account name
* `password` - The password

!!! example

    ``` json
    Char.Login.Credentials { "account": "username", "password": "password" }
    ```

### Server

#### Char.Login

Processes the login request from the client and send back the result.

##### Char.Login.Result

Sent by the server to the client to inform the result of the login attempt.
The payload is a JSON object with the following keys:

* `success` - A string representing a boolean indicating if the login was
              successful: "true" or "false"
* `message` - A string containing a message to the client

!!! example

    ``` json
    Char.Login.Result { "success": "false", "message": "Invalid account name." }
    ```

##### Char.Vitals

Sent by the server to the client to provide the character's vital statistics.
The payload is a JSON object with the following keys:

* `hp` - The character's health points
* `max_hp` - The character's maximum health points
* `hp_ratio` - The character's health points as a ratio of maximum health points
* `sp` - The character's skill points
* `max_sp` - The character's maximum skill points
* `sp_ratio` - The character's skill points as a ratio of maximum skill points
* `mp` - The character's movement points
* `max_mp` - The character's maximum movement points
* `mp_ratio` - The character's movement points as a ratio of maximum movement points

!!! note ""

    Any and all values may be present in the payload, depending on what
    information has changed.

!!! example

    ``` json
    Char.Vitals {
        "hp": "10.0", "max_hp": "100.0", "hp_ratio": "10.0",
        "sp": "15.0", "max_sp": "100.0", "sp_ratio": "15.0",
        "mp": "25.0", "max_mp": "115.0", "mp_ratio": "21.74"
    }
    ```

##### Char.Status

Sent by the server to the client to provide the character's status information.
The payload is a JSON object with the following keys:

* `name` - The character's name
* `xp` - The character's experience points
* `tnl` - The character's time until next level
* `level` - The character's level
* `fill` - The character's fill level
* `capacity` - The character's capacity
* `current_enemy` - The character's current enemy
* `current_enemy_health` - The character's current enemy's health
* `current_enemies` - The character's current enemies
* `seen_enemies` - The character's seen enemies
* `wealth` - The character's wealth

!!! note ""

    Any and all values may be present in the payload, depending on what
    information has changed.

!!! example

    ``` json
    Char.Status {
        "name": "Aragorn",
        "xp": "100.0", "tnl": "200.0", "level": "10",
        "fill": "15.0", "capacity": "100.0",
        "current_enemy": "Goblin", "current_enemy_health": "100.0",
        "current_enemies": [ "Goblin", "Orc" ],
        "seen_enemies": [ "Goblin", "Orc", "Troll" ],
        "wealth": {
            "copper": "10",
            "silver": "10",
            "gold": "10",
            "platinum": "10"
        }
    }
    ```

##### Char.StatusVars

Sent by the server to the client to provide the character's status variables.
These correspond to the variables listed in the `Char.Status` payload and are
provided once, upon initial connection, as a way for the client to get the
key/value pair of label/friendly name of the status variables, perhaps for use
in a GUI.

The payload is a JSON object of key/value pairs where the key is the label of
the status variable and the value is the friendly name of the status variable.

!!! example

    ``` json
    Char.StatusVars {
        "name": "Name",
        "xp": "XP",
        "tnl": "TNL",
        "level": "Level",
        "fill": "Fill",
        "capacity": "Capacity",
        "current_enemy": "Current Enemy",
        "current_enemy_health": "Current Enemy Health",
        "current_enemies": "Current Enemies",
        "seen_enemies": "Seen Enemies",
        "wealth": "Wealth"
    }
    ```
