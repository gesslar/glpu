---
title: base64
---
# base64.c

## base64_decode

### Synopsis

```c
string base64_decode(string source)
```

### Parameters

* `string source` - The Base64 encoded string to be decoded.

### Returns

`string` - The decoded string.

### Description

Decodes a given Base64 encoded string.

## base64_encode

### Synopsis

```c
string base64_encode(mixed source_str)
```

### Parameters

* `mixed source_str` - The string or buffer to be encoded.

### Returns

`string` - The Base64 encoded string.

### Description

Encodes a given string or buffer into Base64 format.

