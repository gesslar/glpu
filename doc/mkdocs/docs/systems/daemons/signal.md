# Signal Daemon (SIGNAL_D)

The Signal daemon is responsible for managing signals that are sent to objects
in the game. Signals are a way to send messages to objects that are not
necessarily in the same room or even in the same area. It is an agnostic
messaging system that allows any object to subscribe to a signal and
receive messages when that signal is sent. Any object can send a signal, and
any object that is listening for that signal will receive it.

All calls to the Signal daemon are done through [simul_efuns](/simul_efun/signal.md).
This ensures that the daemon is the only object that can listen for signal
emissions, send out the signals, and manage the subscriptions.

## Signals

Signals are integer values that are used to identify the type of message that
is being sent. They are used to differentiate between different types of
messages and allow objects to subscribe to only the signals they are
interested in.

To send a signal, you simply call `emit(int signal, mixed args...)`.
This simul_efun does not return any result, due to the nature of signals. It
is a fire-and-forget system; any number of objects could be listening, and
a single return value is not feasible.

### Example

```c
emit(SIG_SYS_BOOT);
```

## Slots

Slots are the subscription points for signals. They are used to determine which
signals an object is listening for. When a signal is sent, the Signal daemon
will check all objects that have subscribed to that signal and send the signal
to them.

To listen for a signal, you use the `slot` simul_efun. You must pass the
integer signal that you want to listen for and the function that you want to
call when that signal is sent. The function must be a valid string that
represents a function in the object that is listening for the signal.

You may only subscribe to a signal with one function per object. Any subsequent
calls to `slot` with the same signal will overwrite the previous function.

The possible return values of `slot` are included in the `include/signal.h`
file.

### Example

```c
void setup() {
    slot(SIG_SYS_BOOT, "on_boot");
}

void on_boot(mixed args...) {
    // Do something when the boot signal is received.
}
```

## Unsubscribing

To stop listening for a signal, you use the `unslot` simul_efun. You must pass
the integer signal that you want to stop listening for. It is unnecessary to
pass the function that you want to stop listening for.

The possible return values of `unslot` are included in the `include/signal.h`
file.

### Example

```c
void clean_up() {
    unslot(SIG_SYS_BOOT);
}
```

## Signal Definitions

There are predefined signals in `include/signal.h` that are used by the game.
A future update will include the ability to define custom signals in the
`adm/etc/config.json` file.

They are set up to be bit-shifted; however, the Signal daemon is not presently
set up to handle bit-shifted signals. This is a future feature that will be
added to the daemon.
