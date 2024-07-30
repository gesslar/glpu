/* npc.c

 Tacitus @ LPUniversity
 28-JUNE-06
 Non-player character

*/

#include <logs.h>

inherit STD_BODY;

void setup_body() ;

void mudlib_setup() {
    enable_commands() ;

    set_name("Generic NPC");
    set_ids(({lower_case(query_name()), query_name(), "npc", "generic npc", "NPC" }));
    set_long(capitalize(query_name()) + " is a generic NPC.");
    set_short(query_name());
    if(!query("env_settings")) set("env_settings", ([]));
    init_vitals() ;

    if(clonep()) {
        add_init("start_heart_beat") ;
        add_hb("stop_heart_beat") ;
        add_module("mob/combat_memory") ;
    }
}

void restore_user() {}
void save_user() {}

int force_me(string cmd) {
    return command(cmd);
}

int player_check() ;
void start_heart_beat() {
    if(player_check())
        set_heart_beat(mud_config("DEFAULT_HEART_RATE"));
}

void stop_heart_beat() {
    if(!player_check() && query_hp() >= 100.0)
        set_heart_beat(0);
}

int player_check() {
    object env ;

    if((env = environment()) && sizeof(present_players(env))) {
        return 1;
    }

    return 0;
}

/* Body Object Functions */
void heart_beat() {
    clean_up_enemies() ;

    if(userp()) {
        if(!interactive(this_object())) {
            if((time() - query("last_login")) > 3600) {
                if(environment())
                    tell_room(environment(), query_name() + " fades out of existance.\n");
                log_file(LOG_LOGIN, query_proper_name() + " auto-quit after 1 hour of net-dead at " + ctime(time()) + ".\n");
                destruct(this_object());
            }
        } else {
            /* Prevent link-dead from idle */
            if(query_idle(this_object()) % 60 == 0 && query_idle(this_object()) > 300
                    && query_env("keepalive") && query_env("keepalive") != "off") {
                telnet_nop() ;
            }
        }
    }

    if(!is_dead() && query_hp() <= 0.0) {
        set_dead(1) ;
        die() ;
        return ;
    }

    heal_tick() ;

    evaluate_heart_beat() ;
}

int is_npc() { return 1; }
