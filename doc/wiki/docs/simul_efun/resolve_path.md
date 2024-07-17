---
layout: default
title: resolve_path
---
# resolve_path.c

## resolve_path

### Synopsis

```c
string resolve_path(string Current, string Next)
```

### Parameters

* `string Current` - The current path.
* `string Next` - The next path to resolve.

### Description

Resolves a given path relative to the current path, handling
special cases such as user directories and relative paths.

