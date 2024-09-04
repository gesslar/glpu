---
title: loot
---
# loot.c

## coin_drop

### Synopsis

```c
void coin_drop(object tp, object source)
```

### Parameters

* `object tp` - The player to drop the coins for.
* `object source` - The source of the coins.

### Description

Drops coins from the coin table from the source into the source.

## determine_value_by_level

### Synopsis

```c
public int determine_value_by_level(int level)
```

### Parameters

* `int level` - The level of the source.

### Returns

`int` - The value of the coins.

### Description

Determines the value of the coins based on the level of the source.

## loot_drop

### Synopsis

```c
void loot_drop(object tp, object source)
```

### Parameters

* `object tp` - The player to drop the loot for.
* `object source` - The source of the loot.

### Description

Drops loot from the loot table from the source into the source.

