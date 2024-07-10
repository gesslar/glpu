#ifndef __DISCORD_BOT_H__
#define __DISCORD_BOT_H__

#include <discord.h>

#define DISCORD_ENDPOINT "https://discord.com/api/"

#define DISCORD_MESSAGE_TYPES ([ \
    EVENT_MESSAGE_CREATE : ([ "endpoint" : "channels/%s/messages", "method" : "POST" ]), \
])

#endif // __DISCORD_BOT_H__
