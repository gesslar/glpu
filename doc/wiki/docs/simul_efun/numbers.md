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

* `float a` - The percentage value.
* `float b` - The whole value.

### Returns

`float` - The value that is `a` percent of `b`.

### Description

Calculates what `a` percent of `b` is.

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

Calculates what percentage `a` is of `b`.

## evaluate_number

### Synopsis

```c
int evaluate_number(int number, string condition)
```

### Parameters

* `int number` - The number to evaluate.
* `string condition` - The condition to evaluate against.

### Returns

`int` - 1 if the condition evaluates to true, 0 otherwise.

### Description

Evaluates a number against a condition. The condition can be a
simple comparison, or a compound condition using `AND` and `OR`.
This system allows for the evaluation of numeric conditions
using a specific set of operators and syntax rules.

### Basic Operators:

* `<` - Less than
* `>` - Greater than
* `<=` - Less than or equal to
* `>=` - Greater than or equal to
* `=` or `==` - Equal to
* `!=` - Not equal to
* `%` - Checks if a number is divisible by the given value.

### Range Operator:

Use a hyphen (`-`) to specify a range, inclusive of both ends.
Example: `5-15` means any number from `5` to `15`, including
`5` and `15`.

### Set Inclusion/Exclusion:

`[a,b,c]` - Checks if a number is one of the listed values.
`![a,b,c]` - Checks if a number is not one of the listed values.

### Logical Operators:

`AND` - Both conditions must be true.
`OR` - At least one condition must be true.

### Grouping

Use parentheses `()` to group conditions and override default
precedence.

### Precedence (from highest to lowest):

* Parentheses `()`
* Basic operators (`<`, `>`, `<=`, `>=`, `=`, `!=`), Range,
Modulo, Set inclusion/exclusion
* `AND`
* `OR`

### Syntax Rules

* No spaces are allowed in the condition string.
* Operators must be used exactly as specified (e.g., >= is
valid, but => is not).
* Set values must be comma-separated without spaces.

#### Example of a complex condition

`(5-15AND%3)OR[20,25,30]`

This checks if a number is between `5` and `15` (inclusive) `AND`
divisible by `3`, `OR` if it's `20`, `25`, or `30`.

Invalid syntax will result in an error, ensuring strict
adherence to these rules.

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

