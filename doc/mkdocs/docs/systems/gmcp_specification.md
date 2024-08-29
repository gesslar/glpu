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

!!! note

    Examples may include the same hash for multiple objects to simplify the
    demonstration. In practice, each object will have a different hash id.

## Supported Packages and modules

* [Core](#core) - Core functionality
* [Char](#char) - Character information
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

#### Char.Items

Sent by the client to request inventory information.

##### Char.Items.Contents

Request a list of all items within a specified container. The payload is a string
reflecting the hash of the container to list items from. The container must be
located in the room the character is in, or in the character's inventory.

!!! example

    ``` json
    Char.Items.Contents "8351cdf67f1b439de383d0bda83318e2"
    ```

##### Char.Items.Inv

Request a list of all items in the character's inventory. There is no payload.

!!! example

    ``` json
    Char.Items.Inv
    ```


##### Char.Items.Room

List items in the room the character is in. There is no payload.

!!! example

    ``` json
    Char.Items.Room
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

#### Char.Items

##### Char.Items.List

List items in a specific inventory or container. The payload is a JSON object
as a string with the following keys:

* `location` - The location of the container to list items from. Possible
               values are:

    * `"inv"` - The character's inventory,
    * `"room"` - The room the character is in,
    * `"hash"` - The object ID of a container in the room or the character's
                 inventory
* `items` - A JSON array of objects representing the items in the container.
            Possible keys for each item are:
    * `"hash"` - The object ID of the item
    * `"name"` - The name of the item
    * `"icon"` - The icon of the item
    * `"id"` - The IDs of the item
    * `"attributes"` - This is an undelimited string containing the attributes
                      of the item.
                      Possible attributes are:
        * `w` - worn
        * `W` - wearable, but not worn
        * `l` - wielded
        * `c` - container
        * `t` - takeable
        * `m` - monster
        * `d` - dead monster

!!! example

    === "inv"

        ``` json
        Char.Items.List {
            "location": "inv",
            "items": [
                {
                    "hash": "8351cdf67f1b439de383d0bda83318e2",
                    "name": "flaming cudgel",
                    "icon": "weapon",
                    "id": [ "cudgel", "flaming cudgel" ],
                    "attributes": "Wt"
                }
            ]
        }
        ```

    === "room"

        ``` json
        Char.Items.List {
            "location": "room",
            "items": [
                {
                    "hash": "8351cdf67f1b439de383d0bda83318e2",
                    "name": "a menacing goblin",
                    "icon": "monster",
                    "id": [ "goblin", "menacing goblin" ],
                    "attributes": "m"
                }
            ]
        }
        ```

    === "hash"

        ``` json
        Char.Items.List {
            "location": "8351cdf67f1b439de383d0bda83318e2",
            "items": [
                {
                    "hash": "8351cdf67f1b439de383d0bda83318e2",
                    "name": "potion of healing",
                    "icon": "potion",
                    "id": [ "potion", "potion of healing" ],
                    "attributes": "t"
                },
                {
                    "hash": "8351cdf67f1b439de383d0bda83318e2",
                    "name": "loaf of bread",
                    "icon": "food",
                    "id": [ "bread", "loaf of bread" ],
                    "attributes": "t"
                }
            ]
        }
        ```

##### Char.Items.Add

Add an item to either the character's inventory or the room the character is in.
The payload is a JSON object in the same format as `Char.Items.List`, except
that it is only one item, and therefore the key will be `item`.

!!! example

    ``` json
    Char.Items.Add {
        "location": "inv",
        "item": {
            "hash": "8351cdf67f1b439de383d0bda83318e2",
            "name": "potion of healing",
            "icon": "potion",
            "id": [ "potion", "potion of healing" ],
            "attributes": "t"
        }
    }
    ```

##### Char.Items.Remove

Remove an item from either the character's inventory or the room the character
is in. The payload is a JSON object in the same format as `Char.Items.Add`.

!!! example

    ``` json
    Char.Items.Remove {
        "location": "room",
        "item": {
            "hash": "8351cdf67f1b439de383d0bda83318e2",
            "name": "potion of healing",
            "icon": "potion",
            "id": [ "potion", "potion of healing" ],
            "attributes": "t"
        }
    }
    ```

##### Char.Items.Update

Update an item in either the character's inventory or the room the character
is in. The payload is a JSON object in the same format as `Char.Items.Add`.

!!! example

    ``` json
    Char.Items.Update {
        "location": "8351cdf67f1b439de383d0bda83318e2",
        "item": {
            "hash": "8351cdf67f1b439de383d0bda83318e2",
            "name": "potion of healing",
            "icon": "potion",
            "id": [ "potion", "potion of healing" ],
            "attributes": "t"
        }
    }
    ```

#### Char.Vitals

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

#### Char.Status

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

#### Char.StatusVars

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

## Room

### Server

#### Room.Info

Sent by the server to the client to provide the room's information. The payload
is a JSON object with the following keys:

* `hash` - The hash of the room
* `area` - The name of the area
* `coords` - The coordinates of the room, as an array of 3 numbers representing
             x, y, and z respectively
* `environment` - The environment of the room
* `exits` - An object containing key/value pairs of exit direction and hash
            of the room to which the exit leads.
* `icon` - The icon of the room
* `size` - The size of the room, as an array of 3 numbers representing width,
          height, and depth respectively
* `type` - The type of the room
* `subtype` - The subtype of the room

!!! example

    ``` json
    Room.Info {
        "area": "Olum",
        "coords": { "-1", "0", "0" },
        "environment": "road",
        "exits": {
            "east": "ce53e6d9d4757b0d7c04223755216865",
            "south": "7ce32c91980d5d8cfbd25a6b36cc134c",
            "west": "d91efff11de437aed3974b4428a320e0"
        },
        "hash": "61660eac2b962b70ab782ee6f697c592",
        "icon": "",
        "name": "A Path Through the Village",
        "size": { "1", "1", "1" },
        "subtype": "",
        "type": "room"
    }
    ```

#### Room.Travel

Sent by the server in response to the `travel` command. The payload is an array
of strings representing the hashes of the rooms to be traveled to.

This can be used by the client to initiate a speedwalk to the target room.
If the character is to be traveling to more than one location, the client
should ensure that a pause is introduced betweeen each destination of at least
2 seconds.

!!! example

    ``` json
    Room.Travel [ "ce53e6d9d4757b0d7c04223755216865", "7ce32c91980d5d8cfbd25a6b36cc134c" ]
    ```
