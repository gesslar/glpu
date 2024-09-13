---
title: random
---
# random.c

## pelement_of

### Synopsis

```c
mixed *pelement_of(mixed seed, mixed *arr)
```

### Parameters

* `mixed seed` - The seed for the random number generator.
* `mixed* arr` - The array to select an element from.

### Returns

`mixed*` - A two element array where the first element is the updated seed and the second is the selected element.

### Description

Selects an element from an array using the xorshift128+
algorithm.

## pshuffle

### Synopsis

```c
mixed *pshuffle(mixed seed, mixed *arr)
```

### Parameters

* `mixed seed` - The seed for the random number generator.
* `mixed* arr` - The array to shuffle.

### Returns

`mixed*` - A two element array where the first element is the updated seed and the second is the shuffled array.

### Description

Shuffles an array using the xorshift128+ algorithm.

## prandom

### Synopsis

```c
int *prandom(mixed seed, int size)
```

### Parameters

* `mixed seed` - The seed for the random number generator.
* `int size` - The upper bound for the random number.

### Returns

`int*` - A two element array where the first element is the updated seed and the second is the random number.

### Description

Generates a random number within a specified range using the
xorshift128+ algorithm.

## element_of_weighted

### Synopsis

```c
mixed element_of_weighted(mapping m)
```

### Parameters

* `mapping m` - The weighted mapping to select from, where keys are the elements and values are their weights.

### Returns

`mixed` - The selected element.

### Description

Selects an element from a weighted mapping based on their weights.

## pelement_of_weighted

### Synopsis

```c
mixed *pelement_of_weighted(mixed seed, mapping weights)
```

### Parameters

* `mixed seed` - The seed for the random number generator.
* `mapping weights` - The weighted mapping to select from, where keys are the elements and values are their weights.

### Returns

`mixed*` - A two element array where the first element is the updated seed and the second is the selected element.

### Description

Selects an element from a weighted mapping using the
xorshift128+ algorithm.

## prandom_clamp

### Synopsis

```c
mixed *prandom_clamp(mixed seed, int min, int max)
```

### Parameters

* `mixed seed` - The seed for the random number generator.
* `int min` - The lower bound (inclusive) of the range.
* `int max` - The upper bound (inclusive) of the range.

### Returns

`mixed*` - A two element array where the first element is the updated seed and the second is the random number.

### Description

Generates a random number within a specified range using the
xorshift128+ algorithm.

## random_clamp

### Synopsis

```c
int random_clamp(int min, int max)
```

### Parameters

* `int min` - The lower bound (inclusive) of the range.
* `int max` - The upper bound (inclusive) of the range.

### Returns

`int` - A random number in the specified range.

### Description

Generates a random integer within a specified range.

## sanitize_seed

### Synopsis

```c
public int *sanitize_seed(mixed seed)
```

### Parameters

* `mixed seed` - The seed to sanitize.

### Returns

`int*` - The sanitized seed.

### Description

Sanitizes the seed for the random number generator. Ensuring
that the seed is a non-zero integer and within the range of
a 64-bit unsigned integer.

## prandom_float

### Synopsis

```c
mixed *prandom_float(mixed seed, float size)
```

### Parameters

* `mixed seed` - The seed for the random number generator.
* `float size` - The upper bound for the random float.

### Returns

`mixed*` - A two element array where the first element is the updated seed and the second is the random float.

### Description

Generates a random float within a specified range using the
xorshift128+ algorithm.

## random_float

### Synopsis

```c
float random_float(mixed upper_bound)
```

### Parameters

* `mixed upper_bound` - The upper bound for the random float.

### Returns

`float` - The random float between 0 and upper_bound.

### Description

Generates a random float between 0 and upper_bound.

