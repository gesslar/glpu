GMCP is a protocol for communication between the MUD server and the MUD client.

It is used to send and receive information about the game world, such as room,
chat, inventory, and other information.

## Defines

All macros used for GMCP are located in `include/gmcp_defines.h` and should
be included when you are sending messages to the client, or writing a module
to process GMCP messages.

In {{ MUD_NAME }}, GMCP is implemented in two ways.

* [Client-initiated](#client-initiated)
* [Server-initiated](#server-initiated)

## Client-initiated

Client-initiated communication is initiated by the MUD client and sent to the
MUD server.

It is handled by standard GMCP module module found in `std/modules/gmcp.c`.

This module is inherited by the body object and the login object. Supporting
GMCP modules are found in `std/modules/gmcp` and are not inherited by any
object, but are instead called by the body or the login object when they
receive the appropriate message.

For example, when you first connect, your MUD client will send a `Core.Hello`
message to initiate communication, providing some information about the
client. It will also send a `Core.Supports.Set` message (and possibly
additional `Core.Supports.Add` messages) to inform the server that the client
supports the modules specified. When received, the login object will call the
Core module (`std/modules/gmcp/Core.c`) to handle the message.

All modules are named as the Package name and contain functions within named
after the module they are handling. For example, the `Core.Hello` message is
handled by the `Hello()` function within `Core.c`.

``` c title="std/modules/gmcp/Core.c"
void Hello(mapping data) {
    object prev = previous_object();
    string key, value ;

    // Core.Hello should only arrive from the login object at the start of the
    // connection.
    if(base_name(prev) != LOGIN_OB)
        return;

    // Convert all keys to lowercase to avoid issues with capitalization.
    foreach(key, value in data) {
        map_delete(data, key);
        data[lower_case(key)] = value;
    }

    prev->set_gmcp_client(data) ;
}
```

Once login has been completed and the body is moved into the game world, the
GMCP information is transferred from the temporary login object to the body.

This process is repeated for each new connection to the MUD, including after
reconecting from having gone linkdead.

## Server-initiated

Server-initiated communication is initiated by the MUD server and sent to the
MUD client.

It is handled by the [GMCP Daemon](daemons/gmcp.md), and controlled by daemon
modules found in `adm/daemons/modules/gmcp`.

These modules are not inherited by anything, but are called by the GMCP Daemon
when the MUD server sends the appropriate message.

For example, if you wish to send the player's health, you can send `Char.Vitals`
message to the daemon, which will identify what package and module is being
sent, and call the appropriate module to handle the message.

``` c title="cmds/std/hp.c"
#include <gmcp_defines.h>

mixed main(object tp, string str) {
    GMCP_D->send_gmcp(tp, GMCP_PKG_CHAR_VITALS, ([
        GMCP_LBL_CHAR_VITALS_HP : sprintf("%f", tp->query_hp())
    ]));
}
```

``` c title="adm/daemons/modules/gmcp/Char.c"
void Vitals(object who, mapping data) {

    // If no data is provided, we will send everything.
    data = data || ([
        GMCP_LBL_CHAR_VITALS_HP : sprintf("%f", who->query_hp()),
        GMCP_LBL_CHAR_VITALS_MAX_HP : sprintf("%f", who->query_max_hp()),
        GMCP_LBL_CHAR_VITALS_HP_RATIO : sprintf("%f", who->query_hp_ratio()),
        GMCP_LBL_CHAR_VITALS_SP : sprintf("%f", who->query_sp()),
        GMCP_LBL_CHAR_VITALS_MAX_SP : sprintf("%f", who->query_max_sp()),
        GMCP_LBL_CHAR_VITALS_SP_RATIO : sprintf("%f", who->query_sp_ratio()),
        GMCP_LBL_CHAR_VITALS_MP : sprintf("%f", who->query_mp()),
        GMCP_LBL_CHAR_VITALS_MAX_MP : sprintf("%f", who->query_max_mp()),
        GMCP_LBL_CHAR_VITALS_MP_RATIO : sprintf("%f", who->query_mp_ratio()),
    ]);

    who->do_gmcp(GMCP_PKG_CHAR_VITALS, data);
}
```
