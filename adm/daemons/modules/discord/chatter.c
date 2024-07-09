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
#include <discord.h>

inherit STD_DISCORD_BOT ;

void setup() {
    set_bot_name("Chatter") ;
    set_intents(
        GATEWAY_INTENT_BITS_DEFAULT |
        GATEWAY_INTENT_BITS_MESSAGE_CONTENT
    ) ;

    _log(2, "Chatter bot setup") ;
    _log(3, "Intents: %d", query_intents()) ;

    start_up() ;
}
