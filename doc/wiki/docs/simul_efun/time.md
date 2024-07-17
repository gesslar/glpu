---
title: time
---
# time.c

## time_frac

### Synopsis

```c
varargs float time_frac(int nanoseconds)
```

### Parameters

* `int nanoseconds` - The time value in nanoseconds. Defaults to the current time in nanoseconds if not provided.

### Description

Converts a time value from nanoseconds to a fractional value in seconds.
This function takes a time value in nanoseconds and converts it to
a fractional value representing seconds by dividing the input by 1,000,000,000.

## time_ms

### Synopsis

```c
varargs int time_ms(int nanoseconds)
```

### Parameters

* `int nanoseconds` - The time value in nanoseconds. Defaults to the current time in nanoseconds if not provided.

### Description

Converts a time value from nanoseconds to milliseconds.
This function takes a time value in nanoseconds and converts it
to milliseconds by dividing the input by 1,000,000.

## ltime

### Synopsis

```c
varargs string ltime(int x, int brief)
```

### Parameters

* `int [x=time()]` - The timestamp to format. Defaults to the current time.
* `int [brief=0]` - If set to 1, returns a brief time format (HH:MM). Otherwise, returns a full time format (HH:MM:SS).

### Description

Returns a formatted time string.

## ldate

### Synopsis

```c
varargs string ldate(int x, int brief)
```

### Parameters

* `int [x=time()]` - The timestamp to format. Defaults to the current time.
* `int [brief=0]` - If set to 1, returns a brief date format (MM-DD). Otherwise, returns a full date format (YYYY-MM-DD).

### Description

Returns a formatted date string.

