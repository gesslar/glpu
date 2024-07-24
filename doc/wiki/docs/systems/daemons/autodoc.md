# Autodoc System (AUTODOC_D)

## Overview
The autodoc system is a powerful documentation generator designed for gLPU. It
parses JSDoc-style comments in LPC source files and generates structured
documentation for both the mudlib and a wiki.

## Initialization
The system starts by loading configuration settings:

 * `AUTODOC_ROOT` - Directory for mudlib documentation
 * `WIKI_DOC_ROOT` - Directory for wiki documentation
 * `AUTODOC_SOURCE_DIRS` - Directories to scan for source files

## Scanning Process

The scanning process is synchronous and occurs in two phases:

1. Directory Scanning - Iterates through `AUTODOC_SOURCE_DIRS` and collects
   all `.c` files in each directory.
2. File Parsing - Parses each collected file for JSDoc-style comments and
   extracts function definitions and associated documentation.

## Parsing Logic
The system recognizes the following JSDoc tags:

* `@description`
* `@def`
* `@param`
* `@returns`
* `@example`

It uses regular expressions to identify these tags and extract their content.

# Documentation Generation

After parsing, the system generates two types of documentation:

* Mudlib Documentation - Structured format for internal use Organized by
  function type and name.
* Wiki Documentation - Markdown format for easy reading Includes an index and
  individual function pages.

# Usage

To start the autodoc process, user the `autodoc` command with the `scan` argument. You will see output that looks similar to this:

    • Starting autodoc scan
    • Autodoc has completed.
    • Scan time: 0.72s
    • Directories scanned: 2
    • Files scanned: 60

### Writing Documentation
To document your functions, use JSDoc-style comments above your function definitions:
```
/**
 * @simul_efun add_numbers
 * @description Calculates the sum of two numbers
 * @param {int} a - The first number
 * @param {int} b - The second number
 * @returns {int} The sum of a and b
 * @example
 * int result = add_numbers(5, 3);
 * // result is now 8
 */
int add_numbers(int a, int b) {
    return a + b;
}
```

## Output

### Mudlib Documentation

Mudlib documentation is generated in a structured format, organized by function
type and name. This is useful for programmatic access to documentation.

### Wiki Documentation

Wiki documentation is generated in Markdown format, which includes:

* An index page for each function type
* Individual pages for each source file, containing:
  * Function synopses
  * Parameter descriptions
  * Return value descriptions
  * Detailed descriptions
  * Examples (if provided)

## Best Practices

* Be consistent in your documentation style
* Always include a @description for each function
* Document all parameters using @param
* Specify return values with @returns
* Provide examples for complex functions using @example

## Troubleshooting
If you encounter issues:

* Check the system log for error messages
* Ensure all required configuration settings are properly defined
* Verify that your JSDoc comments are correctly formatted

By following these guidelines and utilizing the autodoc system, you can
maintain comprehensive and up-to-date documentation for your MUD, improving
code maintainability and easing the onboarding process for new developers.
