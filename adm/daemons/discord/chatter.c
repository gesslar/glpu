/**
 * @file /adm/daemons/modules/chatter.c
 * @description Chatter Discord Bot Module
 *
 * @created 2024/07/08 - Gesslar
 * @last_modified 2024/07/08 - Gesslar
 *
 * @history
 * 2024/07/08 - Gesslar - Created
 */

#include <daemons.h>
#include <discord_bot.h>

inherit STD_DISCORD_BOT ;

void setup() {
    set_bot_name("Chatter") ;
    set_intents(
        GATEWAY_INTENT_BITS_DEFAULT |
        GATEWAY_INTENT_BITS_MESSAGE_CONTENT
    ) ;

    _log(2, "Chatter bot setup") ;
    _log(3, "Intents: %d", query_intents()) ;

    start_bot() ;
}

void reply_to_message(mapping message) ;

void handle_message_create(mixed data) {
    mapping payload ;
    _log(1, "Message from %s: %s", data["author"]["username"], data["content"]) ;

    payload = ([
        "content": "Hello, " + data["author"]["username"] + "!",
        "channel_id": data["channel_id"]
    ]) ;

    reply_to_message(payload) ;
}

void reply_to_message(mapping payload) {
    _log(2, "Replying to message with %O", payload) ;
    call_out_walltime((: discord_send_text, EVENT_MESSAGE_CREATE, payload :), 2.0) ;
}
