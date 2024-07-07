// intents.h

#ifndef INTENTS_H
#define INTENTS_H

// Individual intent bit values
#define INTENT_GUILDS                     (1 << 0)  // 1
#define INTENT_GUILD_MEMBERS              (1 << 1)  // 2
#define INTENT_GUILD_BANS                 (1 << 2)  // 4
#define INTENT_GUILD_EMOJIS_AND_STICKERS  (1 << 3)  // 8
#define INTENT_GUILD_INTEGRATIONS         (1 << 4)  // 16
#define INTENT_GUILD_WEBHOOKS             (1 << 5)  // 32
#define INTENT_GUILD_INVITES              (1 << 6)  // 64
#define INTENT_GUILD_VOICE_STATES         (1 << 7)  // 128
#define INTENT_GUILD_PRESENCES            (1 << 8)  // 256
#define INTENT_GUILD_MESSAGES             (1 << 9)  // 512
#define INTENT_GUILD_MESSAGE_REACTIONS    (1 << 10) // 1024
#define INTENT_GUILD_MESSAGE_TYPING       (1 << 11) // 2048
#define INTENT_DIRECT_MESSAGES            (1 << 12) // 4096
#define INTENT_DIRECT_MESSAGE_REACTIONS   (1 << 13) // 8192
#define INTENT_DIRECT_MESSAGE_TYPING      (1 << 14) // 16384
#define INTENT_MESSAGE_CONTENT            (1 << 15) // 32768
#define INTENT_GUILD_SCHEDULED_EVENTS     (1 << 16) // 65536

// Combined intent values
#define GATEWAY_INTENT_BITS_GUILDS \
    (INTENT_GUILDS)

#define GATEWAY_INTENT_BITS_GUILD_MEMBERS \
    (INTENT_GUILD_MEMBERS)

#define GATEWAY_INTENT_BITS_GUILD_MESSAGES \
    (INTENT_GUILD_MESSAGES)

#define GATEWAY_INTENT_BITS_MESSAGE_CONTENT \
    (INTENT_MESSAGE_CONTENT)

// Example combined intents for convenience
#define GATEWAY_INTENT_BITS_DEFAULT \
    (INTENT_GUILDS | INTENT_GUILD_MESSAGES)

#define GATEWAY_INTENT_BITS_ALL \
    (INTENT_GUILDS | INTENT_GUILD_MEMBERS | INTENT_GUILD_BANS | INTENT_GUILD_EMOJIS_AND_STICKERS | \
     INTENT_GUILD_INTEGRATIONS | INTENT_GUILD_WEBHOOKS | INTENT_GUILD_INVITES | INTENT_GUILD_VOICE_STATES | \
     INTENT_GUILD_PRESENCES | INTENT_GUILD_MESSAGES | INTENT_GUILD_MESSAGE_REACTIONS | INTENT_GUILD_MESSAGE_TYPING | \
     INTENT_DIRECT_MESSAGES | INTENT_DIRECT_MESSAGE_REACTIONS | INTENT_DIRECT_MESSAGE_TYPING | INTENT_MESSAGE_CONTENT | \
     INTENT_GUILD_SCHEDULED_EVENTS)

#endif // INTENTS_H
