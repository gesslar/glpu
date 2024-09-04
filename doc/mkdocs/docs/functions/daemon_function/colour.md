---
title: colour
---
# colour.c

## token_to_colour

### Synopsis

```c
string token_to_colour(string token)
```

### Parameters

* `string token` - The colour token to convert.

### Returns

{string} The corresponding colour code.

### Description

Converts a colour token to its corresponding colour code.

## colourp

### Synopsis

```c
int colourp(string text)
```

### Parameters

* `string text` - The text to check for colour codes.

### Returns

{int} 1 if colour codes are present, 0 otherwise.

### Description

Checks if a string contains colour codes.

## get_colour_list

### Synopsis

```c
string get_colour_list()
```

### Returns

{string} A formatted string containing all colour codes and their visual representation.

### Description

Generates a formatted list of all available colours.

## is_too_dark

### Synopsis

```c
int is_too_dark(string colour)
```

### Parameters

* `string colour` - The colour code to check.

### Returns

{int} 1 if the colour is too dark, 0 otherwise.

### Description

Checks if a given colour is considered too dark.

## substitute_too_dark

### Synopsis

```c
string substitute_too_dark(string text)
```

### Parameters

* `string text` - The colour code to potentially substitute.

### Returns

{string} The substituted colour code if too dark, or the original if not.

### Description

Substitutes a colour if it's considered too dark.

## substitute_colour

### Synopsis

```c
public string substitute_colour(string text, string mode)
```

### Parameters

* `string text` - The text containing colour tokens.
* `string mode` - The mode for colour substitution (plain, vt100, high, low).

### Returns

{string} The text with colour tokens replaced by appropriate codes.

### Description

Substitutes colour tokens in text with appropriate colour codes.

## wrap

### Synopsis

```c
public string wrap(string str, int wrap_at, int indent_at)
```

### Parameters

* `string str` - The text to wrap.
* `int wrap_at` - The column at which to wrap the text.
* `int indent_at` - The number of spaces to indent wrapped lines.

### Returns

{string} The wrapped text.

### Description

Wraps text to a specified width, preserving colour codes.

## colour_to_rgb

### Synopsis

```c
public int *colour_to_rgb(int colour_code)
```

### Parameters

* `int colour_code` - The 256 colour code (0-255).

### Returns

{int[]} An array containing the RGB values.

### Description

Converts a 256 colour code to an RGB tuple.

## body_colour_replace

### Synopsis

```c
public string body_colour_replace(object body, string text, int message_type)
```

### Parameters

* `object body` - The body object with colour preferences.
* `string text` - The text to process.
* `int message_type` - The type of message being processed.

### Returns

{string} The text with colours replaced according to preferences.

### Description

Replaces colour codes in text based on body preferences and message type.

## colour_to_greyscale

### Synopsis

```c
int colour_to_greyscale(int colour_code)
```

### Parameters

* `int colour_code` - The colour code to convert.

### Returns

{int} The greyscale equivalent of the colour code.

### Description

Converts a colour code to its greyscale equivalent.

## resync

### Synopsis

```c
void resync()
```

### Description

Reloads all colour definitions.

