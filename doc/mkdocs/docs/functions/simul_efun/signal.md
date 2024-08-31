---
title: signal
---
# signal.c

## unslot

### Synopsis

```c
int unslot(int sig)
```

### Parameters

* `int sig` - signal number

### Returns

`int` - `SIG_SLOT_OK` if the slot was unregistered successfully. See `include/signal.h` for other return values.

### Description

Unregister a slot for a signal.

## emit

### Synopsis

```c
void emit(int sig, mixed arg...)
```

### Parameters

* `int sig` - signal number
* `mixed... arg` - arguments to pass to the signal

### Description

Emit a signal to all objects that have registered a slot for
the signal.

## slot

### Synopsis

```c
int slot(int sig, string func)
```

### Parameters

* `int sig` - signal number
* `string func` - function to call when the signal is emitted

### Returns

`int` - `SIG_SLOT_OK` if the slot was registered successfully. See `include/signal.h` for other return values.

### Description

Register a slot for a signal.

## signal_d

### Synopsis

```c
object signal_d()
```

### Returns

`object` - signal daemon object

### Description

Get the signal daemon object.

