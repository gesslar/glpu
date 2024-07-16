# json_decode

## Synopsis

```c
mixed json_decode(string text)
```

### Parameters

* `string text` - The JSON string to deserialize.

## Description

Deserializes a JSON string into an LPC value.

# json_encode

## Synopsis

```c
varargs string json_encode(mixed value, mixed* pointers)
```

### Parameters

* `mixed value` - The LPC value to serialize.
* `mixed[] [pointers]` - An optional array of pointers to handle circular references.

## Description

Serializes an LPC value into a JSON string.

