# tell_up

## SYNOPSIS

    varargs void tell_up(object ob, string str, int msg_type, mixed exclude)

### PARAMETERS

    object ob - The object to send the message from.
    string str - The message string to send.
    int [msg_type] - The message type, combined with UP_MSG.
    mixed [exclude] - The objects to exclude from receiving the message.

## DESCRIPTION

Sends a message upward through the containment hierarchy, such as
from an object to its container, and further up to the room or
environment.
