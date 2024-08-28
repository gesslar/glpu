---
title: random
---
# random.c

## pelement_of

### Synopsis

```c
mixed *pelement_of(int seed, mixed *arr)
```

### Parameters

* `int seed` - The seed for random number generation.
* `mixed* arr` - The array to select from.

### Returns

`mixed*` - An array where the first element is the new seed and the second element is the randomly selected array element.

### Description

Selects a random element from an array, along with a new seed.

## prandom

### Synopsis

```c
int *prandom(int seed, int max)
```

### Parameters

* `int seed` - The seed for random number generation.
* `int max` - The upper bound (exclusive) for the random number.

### Returns

`int*` - An array where the first element is the new seed and the second element is the random number.

### Description

Generates a random integer between 0 and max-1, along with a new seed.

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
mixed *pelement_of_weighted(int seed, mapping weights)
```

### Parameters

* `int seed` - The seed for random number generation.
* `mapping weights` - A mapping where keys are items and values are weights.

### Returns

`mixed*` - An array where the first element is the new seed and the second element is the randomly selected item based on the weights.

### Description

Selects a random element based on weighted probabilities, along with a new seed.

## prandom_range

### Synopsis

```c
int *prandom_range(int seed, int min, int max)
```

### Parameters

* `int seed` - The seed for random number generation.
* `int min` - The lower bound (inclusive) of the range.
* `int max` - The upper bound (inclusive) of the range.

### Returns

`int*` - An array where the first element is the new seed and the second element is the random number in the specified range.

### Description

Generates a random integer within a specified range, along with a new seed.

## random_range

### Synopsis

```c
int random_range(int min, int max)
```

### Parameters

* `int min` - The lower bound (inclusive) of the range.
* `int max` - The upper bound (inclusive) of the range.

### Returns

`int` - A random number in the specified range.

### Description

Generates a random integer within a specified range.

## prandom_float

### Synopsis

```c
mixed *prandom_float(int seed)
```

### Parameters

* `int seed` - The seed for random number generation.

### Returns

`mixed*` - An array where the first element is the new seed and the second element is the random float.

### Description

Generates a random float between 0 and 1, along with a new seed.

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

