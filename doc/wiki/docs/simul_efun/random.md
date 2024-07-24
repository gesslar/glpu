---
title: random
---
# random.c

## random_element

### Synopsis

```c
mixed *random_element(int seed, mixed *arr)
```

### Parameters

* `int seed` - The seed for random number generation.
* `mixed* arr` - The array to select from.

### Returns

`mixed*` - An array where the first element is the new seed and the second element is the randomly selected array element.

### Description

Selects a random element from an array, along with a new seed.

## random_weighted

### Synopsis

```c
mixed *random_weighted(int seed, mapping weights)
```

### Parameters

* `int seed` - The seed for random number generation.
* `mapping weights` - A mapping where keys are items and values are weights.

### Returns

`mixed*` - An array where the first element is the new seed and the second element is the randomly selected item based on the weights.

### Description

Selects a random element based on weighted probabilities, along with a new seed.

## random_range

### Synopsis

```c
int *random_range(int seed, int min, int max)
```

### Parameters

* `int seed` - The seed for random number generation.
* `int min` - The lower bound (inclusive) of the range.
* `int max` - The upper bound (inclusive) of the range.

### Returns

`int*` - An array where the first element is the new seed and the second element is the random number in the specified range.

### Description

Generates a random integer within a specified range, along with a new seed.

## random_float

### Synopsis

```c
mixed *random_float(int seed)
```

### Parameters

* `int seed` - The seed for random number generation.

### Returns

`mixed*` - An array where the first element is the new seed and the second element is the random float.

### Description

Generates a random float between 0 and 1, along with a new seed.

## random_number

### Synopsis

```c
int *random_number(int seed, int max)
```

### Parameters

* `int seed` - The seed for random number generation.
* `int max` - The upper bound (exclusive) for the random number.

### Returns

`int*` - An array where the first element is the new seed and the second element is the random number.

### Description

Generates a random integer between 0 and max-1, along with a new seed.

