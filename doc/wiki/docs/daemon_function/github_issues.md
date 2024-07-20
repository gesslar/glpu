---
title: github_issues
---
# github_issues.c

## process_backlog

### Synopsis

```c
public nomask void process_backlog()
```

### Description

Process the backlog of requests

## create_issue

### Synopsis

```c
varargs mixed create_issue(string type, string title, string body, mixed *callback)
```

### Parameters

* `string type` - The type of issue to create
* `string title` - The title of the issue
* `string body` - The body of the issue
* `mixed callback` - The callback function to call when the request is complete

### Description

Create a new issue in the GitHub repository

