# tell_down

## SYNOPSIS

    varargs void tell_down(object ob, string str, int msg_type, mixed exclude)

### PARAMETERS

    object ob - The object to send the message from.
    string str - The message string to send.
    int [msg_type] - The message type, combined with DOWN_MSG.
    mixed [exclude] - The objects to exclude from receiving the message.

## DESCRIPTION

Sends a message downward through the containment hierarchy, such
as from a container to all objects it contains.
