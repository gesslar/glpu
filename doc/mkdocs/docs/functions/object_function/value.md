---
title: value
---
# value.c

## adjust_value

### Synopsis

```c
void adjust_value(int value)
```

### Parameters

* `int value` - The value to add.

### Description

Add to the value of the object. The value is added as an
integer representing the value in the lowest denomination of
currency.

## query_value

### Synopsis

```c
int query_value()
```

### Returns

`int` - The value of the object.

### Description

Query the value of the object. The value is returned as an
integer representing the value in the lowest denomination of
currency.

## set_value

### Synopsis

```c
void set_value(int value)
```

### Parameters

* `int value` - The value to set.

### Description

Set the value of the object. The value is stored as an integer
representing the value in the lowest denomination of currency.

