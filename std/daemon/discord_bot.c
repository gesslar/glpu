/**
 * @file /std/daemon/discord_bot.c
 * @description Inheritable for Discord bots
 *
 * @created 2024/07/08 - Gesslar
 * @last_modified 2024/07/08 - Gesslar
 *
 * @history
 * 2024/07/08 - Gesslar - Created
 */

#include <daemons.h>
#include <discord.h>
#include <origin.h>

inherit STD_DAEMON ;
inherit M_LOG ;

public nomask void start_up() ;
public nomask void started(int fd) ;
public nomask void shutdown() ;
protected nomask void set_bot_name(string name) ;
public nomask string query_bot_name() ;
public nomask string query_token() ;
protected nomask void set_intents(int intents) ;
public nomask int query_intents() ;
protected nomask int is_setup() ;

private nomask nosave mapping bot_data = ([]) ;

void mudlib_setup() {
    if(append(base_name(), ".c") != __FILE__) {
        string this_file_name = query_file_name() ;
        string token = mud_config("DISCORD_BOT_TOKENS")[this_file_name] ;

        if(!stringp(token))
            error("No token found for bot "+this_file_name) ;

        set_log_level(1) ;

        bot_data["token"] = token ;
    }
}

public nomask void start_up() {
    mixed *cb ;
    if(append(base_name(), ".c") != __FILE__) {
        if(!is_setup()) {
            _log(2, "Bot not setup") ;
            return ;
        }

        _log(1, "Starting %s bot", query_bot_name()) ;
        cb = assemble_call_back((: started :)) ;
        call_out((: DISCORD_D->start_bot(this_object(), $(cb)) :), 2.0) ;
    }
}

void started(mapping data) {
    if(!mapp(data)) {
        _log(2, "Invalid data received from Discord") ;
        return ;
    }

    if(nullp(data["fd"])) {
        _log(2, "Invalid fd received from Discord") ;
        return ;
    }

    if(data["fd"] < 0) {
        if(data["error"]) {
            _log(2, "Error starting bot: %O", data["error"]) ;
        } else {
            _log(2, "Unknown error starting bot") ;
        }
        return ;
    }

    _log(2, "Bot %O started", query_bot_name()) ;

    bot_data["fd"] = data["fd"] ;
}

void shutdown() {
    if(!is_setup()) {
        _log(2, "Bot not setup") ;
        return ;
    }

    _log(1, "Shutdown of %s bot has been requested", query_bot_name()) ;
    DISCORD_D->stop_bot(bot_data["fd"]) ;
}

void handle_shutdown() {
    _log(1, "%s bot has been shut down", query_bot_name()) ;
    bot_data["fd"] = null ;
}

/**
 * These functions are the event handlers for the Discord bot
 */

void handle_connected() {
    _log(1, "%s bot is now connected to Discord", query_bot_name()) ;
}

// EVENT HANDLING
protected handle_guild_create(int fd, mapping payload) ;
protected handle_message_create(int fd, mapping payload) ;
protected handle_ready(int fd, mapping payload) ;

public nomask handle_event(int fd, string event, mixed data) {
    _log(4, "Starting handle_event(%O, %O, %O)", fd, event, data) ;

    if(base_name(previous_object()) != DISCORD_D)
        _log(2, "Illegal call to handle_event()") ;
    else
        _log(3, "handle_event() called from %O", previous_object()) ;

    if(function_exists("handle_"+lower_case(event)))
        catch(call_other(this_object(), "handle_"+lower_case(event), fd, data)) ;
    else
        _log(2, "No handler for event %O", event) ;
}

// Handle the READY event
void handle_ready(int fd, mapping payload) {
    _log(2, "Received READY event") ;

    // Do something with the READY event
}

void handle_guild_create(int fd, mapping payload) {
    _log(2, "Received GUILD_CREATE event") ;

    // Do something with the GUILD_CREATE event
}

void handle_message_create(int fd, mapping payload) {
    _log(2, "Received MESSAGE_CREATE event") ;

    // Do something with the MESSAGE_CREATE event
}

/**
 *  The following functions are support functions for the bot
 */

protected nomask void set_bot_name(string name) {
    if(!stringp(name)) error("Invalid argument 1 to set_bot_name()");

    set_log_prefix("("+name+")") ;

    _log(3, "Setting bot name to %O", name) ;

    bot_data["name"] = name ;
}

public nomask string query_bot_name() {
    _log(4, "query_bot_name() called from %O", previous_object()) ;
    return bot_data["name"] ;
}

public nomask string query_token() {
    object ob ;

    _log(4, "query_token() called from %O", previous_object()) ;

    if(origin() != ORIGIN_LOCAL && base_name(previous_object()) != DISCORD_D)
        error("Illegal call to query_token()") ;

    return bot_data["token"] ;
}

protected nomask void set_intents(int intents) {
    if(!stringp(bot_data["name"])) error("Bot name must be set before intents") ;

    _log(3, "Setting intents to %O", intents) ;

    if(nullp(intents)) error("Invalid argument 1 to set_intents()");

    bot_data["intents"] = intents ;
}

public nomask int query_intents() {
    _log(4, "query_intents() called from %O", previous_object()) ;
    return bot_data["intents"] ;
}

protected nomask int is_setup() {
    return stringp(bot_data["token"]) &&
           stringp(bot_data["name"]) &&
           intp(bot_data["intents"]) ;
}
