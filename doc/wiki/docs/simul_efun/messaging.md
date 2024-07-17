---
title: messaging
---
# messaging.c

## tell_all

### Synopsis

```c
varargs void tell_all(object ob, string str, int msg_type, mixed exclude)
```

### Parameters

* `object ob` - The object to send the message from.
* `string str` - The message string to send.
* `int [msg_type]` - The message type, combined with ALL_MSG.
* `mixed [exclude]` - The objects to exclude from receiving the message.

### Description

Sends a message to all objects in the same environment, regardless
of their position in the containment hierarchy.

## tell

### Synopsis

```c
varargs void tell(object ob, string str, int msg_type)
```

### Parameters

* `object ob` - The object to send the message to.
* `string str` - The message string to send.
* `int [msg_type]` - The message type.

### Description

Sends a direct message to the specified object without considering
containment hierarchy.

## tell_down

### Synopsis

```c
varargs void tell_down(object ob, string str, int msg_type, mixed exclude)
```

### Parameters

* `object ob` - The object to send the message from.
* `string str` - The message string to send.
* `int [msg_type]` - The message type, combined with DOWN_MSG.
* `mixed [exclude]` - The objects to exclude from receiving the message.

### Description

Sends a message downward through the containment hierarchy, such
as from a container to all objects it contains.

## tell_up

### Synopsis

```c
varargs void tell_up(object ob, string str, int msg_type, mixed exclude)
```

### Parameters

* `object ob` - The object to send the message from.
* `string str` - The message string to send.
* `int [msg_type]` - The message type, combined with UP_MSG.
* `mixed [exclude]` - The objects to exclude from receiving the message.

### Description

Sends a message upward through the containment hierarchy, such as
from an object to its container, and further up to the room or
environment.

## tell_direct

### Synopsis

```c
varargs void tell_direct(object ob, string str, int msg_type)
```

### Parameters

* `object ob` - The object to send the message to.
* `string str` - The message string to send.
* `int [msg_type]` - The message type, combined with DIRECT_MSG.

### Description

Sends a direct message to the specified object without considering
containment hierarchy.

