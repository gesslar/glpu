---
title: numbers
---
# numbers.c

## percent_of

### Synopsis

```c
float percent_of(float a, float b)
```

### Parameters

* `float a` - The part value.
* `float b` - The whole value.

### Returns

`float` - The percentage of `a` in `b`.

### Description

Calculates what percentage `a` is of `b`.

## remainder

### Synopsis

```c
varargs float remainder(mixed a, mixed b)
```

### Parameters

* `mixed a` - The dividend.
* `mixed b` - The divisor.

### Returns

`float` - The remainder of `a` divided by `b`.

### Description

Calculates the remainder of `a` divided by `b`.

## percent

### Synopsis

```c
float percent(float a, float b)
```

### Parameters

* `float a` - The part value.
* `float b` - The whole value.

### Returns

`float` - The percentage of `a` out of `b`.

### Description

Calculates the percentage of `a` out of `b`.

## range

### Synopsis

```c
float range(float min, float max, float val)
```

### Parameters

* `float min` - The minimum value.
* `float max` - The maximum value.
* `float val` - The value to check.

### Returns

`float` - The value, constrained within the range of `min` to `max`.

### Description

Ensures a value is within a specified range.

## sum

### Synopsis

```c
int sum(mixed *arr)
```

### Parameters

* `mixed* arr` - The array of numbers to sum.

### Returns

`int` - The sum of all elements in the array.

### Description

Calculates the sum of all elements in an array.

