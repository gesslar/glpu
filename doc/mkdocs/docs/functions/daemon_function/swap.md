---
title: swap
---
# swap.c

## swap_out

### Synopsis

```c
mixed swap_out(string label)
```

### Parameters

* `string label` - The identifier for the stored value.

### Returns

{mixed} The value associated with the given label, or 0 if not found.

### Description

Retrieves and removes a value from the swap daemon by its label.

## swap_in

### Synopsis

```c
void swap_in(string label, mixed value)
```

### Parameters

* `string label` - The identifier for the stored value.
* `mixed value` - The value to be stored.

### Description

Stores a value in the swap daemon under a given label.

