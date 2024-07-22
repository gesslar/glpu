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

### Description

Register a slot for a signal.

## signal_d

### Synopsis

```c
object signal_d()
```

### Description

Get the signal daemon object.

