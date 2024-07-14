# tell

## SYNOPSIS

    varargs void tell(object ob, string str, int msg_type)

### PARAMETERS

    object ob - The object to send the message to.
    string str - The message string to send.
    int [msg_type] - The message type.

## DESCRIPTION

Sends a direct message to the specified object without considering
containment hierarchy.
