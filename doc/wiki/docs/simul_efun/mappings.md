---
layout: default
title: mappings
---
# mappings.c

## element_of_weighted

### Synopsis

```c
mixed element_of_weighted(mapping m)
```

### Parameters

* `mapping m` - The weighted mapping to select from, where keys are the elements and values are their weights.

### Description

Selects an element from a weighted mapping based on their weights.

## pretty_map

### Synopsis

```c
string pretty_map(mapping map)
```

### Parameters

* `mapping map` - The mapping to format.

### Description

Returns a formatted string representation of a mapping, removing
any size annotations.

