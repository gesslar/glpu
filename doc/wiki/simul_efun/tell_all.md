# tell_all

## SYNOPSIS

    varargs void tell_all(object ob, string str, int msg_type, mixed exclude)

### PARAMETERS

    object ob - The object to send the message from.
    string str - The message string to send.
    int [msg_type] - The message type, combined with ALL_MSG.
    mixed [exclude] - The objects to exclude from receiving the message.

## DESCRIPTION

Sends a message to all objects in the same environment, regardless
of their position in the containment hierarchy.
