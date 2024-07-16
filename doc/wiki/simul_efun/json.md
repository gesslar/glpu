## json_decode

## SYNOPSIS

```c
mixed json_decode(string text)
```

### PARAMETERS

* `string text` - The JSON string to deserialize.

## DESCRIPTION

Deserializes a JSON string into an LPC value.

## json_encode

## SYNOPSIS

```c
varargs string json_encode(mixed value, mixed* pointers)
```

### PARAMETERS

* `mixed value` - The LPC value to serialize.
* `mixed[] [pointers]` - An optional array of pointers to handle circular references.

## DESCRIPTION

Serializes an LPC value into a JSON string.

