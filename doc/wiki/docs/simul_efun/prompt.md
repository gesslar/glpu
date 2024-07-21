---
title: prompt
---
# prompt.c

## prompt_colour

### Synopsis

```c
varargs void prompt_colour(object body, mixed *cb, string prompt)
```

### Parameters

* `object body` - The user's body object.
* `mixed* cb` - The callback array.
* `string prompt` - The prompt message.

### Description

Prompt the user for a colour. The user will be presented with a
list of colours to choose from. The user can also enter a
number corresponding to an xterm 256 colour, or the word "plain"
to select no colour.
Upon selection, the callback function will be called with the
selected colour as the argument.

## prompt_password

### Synopsis

```c
void prompt_password(object body, int attempts, mixed *cb)
```

### Parameters

* `object body` - The user's body object.
* `int attempts` - The number of attempts allowed.
* `mixed* cb` - The callback array.

### Description

Prompt the user for a password.

