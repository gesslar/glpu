---
title: boot
---
# boot.c

## event_boot

### Synopsis

```c
void event_boot(object prev)
```

### Parameters

* `object prev` - The previous object that called this function.

### Description

Called when the mud boots up. Increments the boot number and
saves the data. Must be included in `adm/etc/preload`.

