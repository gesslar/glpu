# # Boot Daemon (BOOT_D)

The Boot daemon is a fairly straightforward daemon that is responsible for
incrementing the boot number and providing a way to access it. The boot number
is used to track the number of times the game has been booted. This is useful
if you want to have certain things only happen or be available during a
specific boot number, like the current one.

There is no interaction required and the only available api is to
[query the current boot number](../../daemon_function/boot.md#query_boot_number).
