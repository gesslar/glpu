---
title: autodoc
---
# autodoc.c

## autodoc_scan

### Synopsis

```c
public nomask mixed autodoc_scan()
```

### Returns

`mixed` - 1 if the scan was started successfully, an error message if the scan is already running.

### Description

Start the autodoc scan process. This will trigger the daemon to
scan the directories specified in the AUTODOC_SOURCE_DIRS
configuration variable for LPC source files and parse the
JSDoc-style comments in those files.

The parsed documentation will be written to the
AUTODOC_ROOT directory in a structured format for use in
generating documentation for the mudlib.

The parsed documentation will also be written to the
WIKI_DOC_ROOT directory in a structured format for use in
generating documentation for the wiki.

