#ifndef __COLOUR_H__
#define __COLOUR_H__

#define RESET_REGEX \
"\\{\\{res\\}\\}"   "|" \
"\\{\\{RES\\}\\}"

#define ATTRIBUTE_REGEX \
"\\{\\{(re|it|ul|fl|st|bl|di|ol][0-1])\\}\\}"

// FG: <#000> - <#fff> | BG: [#000] - [#fff]
// FG: <#000000> - <#ffffff> | BG: [#000000] - [#ffffff]
#define TRUE_COLOUR_REGEX           \
"\\{\\{([a-fA-F0-9]{6})\\}\\}"  "|" \
"\\{\\{([a-fA-F0-9]{3})\\}\\}"

#define COLOUR_REGEX      \
RESET_REGEX           "|" \
ATTRIBUTE_REGEX       "|" \
TRUE_COLOUR_REGEX

#define SYSTEM_OK       "{{009966}}"
#define SYSTEM_ERROR    "{{CC0000}}"
#define SYSTEM_WARNING  "{{FF9900}}"
#define SYSTEM_INFO     "{{FFFF66}}"
#define SYSTEM_QUERY    "{{0066FF}}"

#endif // __COLOUR_H__
