/**
 * @file /adm/daemons/modules/chatter.c
 * @description Chatter Discord Bot Example
 *
 * @created 2024/07/08 - Gesslar
 * @last_modified 2024/07/08 - Gesslar
 *
 * @history
 * 2024/07/08 - Gesslar - Created
 */

#include <discord_bot.h>

inherit STD_DISCORD_BOT ;

void setup() {
    set_bot_name("Chatter") ;
    set_intents(
        GATEWAY_INTENT_BITS_DEFAULT |
        GATEWAY_INTENT_BITS_MESSAGE_CONTENT
    ) ;
                    /* #bots                  #general             */
    restrict_channels("1260442252929404989") ;

    set_log_level(0) ;
    start_bot() ;
}

void handle_message_create(mixed data) {
    mapping payload ;

    payload = ([
        "content": "Hello, " + data["author"]["username"] + "!",
        "channel_id": data["channel_id"]
    ]) ;

    call_out_walltime((:
        discord_send_text, EVENT_MESSAGE_CREATE, payload
    :), 2.0) ;
}
