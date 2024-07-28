/* npc.c

 Tacitus @ LPUniversity
 28-JUNE-06
 Non-player character

*/

inherit STD_BODY;

void setup_body() ;

void mudlib_setup() {
    enable_commands() ;

    _debug("Mudlib setup = %O", this_object());

    set_name("Generic NPC");
    set_ids(({lower_case(query_name()), query_name(), "npc", "generic npc", "NPC" }));
    set_long(capitalize(query_name()) + " is a generic NPC.");
    set_short(query_name());
    add_init("start_heart_beat") ;
    add_hb("stop_heart_beat") ;
    init_vitals() ;
}

void restore_user() {}
void save_user() {}

int force_me(string cmd) {
    return command(cmd);
}

int player_check() ;
void start_heart_beat() {
    if(player_check()) {
        _debug("Players in environment, starting heartbeat = %O", this_object());
        set_heart_beat(mud_config("DEFAULT_HEART_RATE"));
    }
}

void stop_heart_beat() {
    if(!player_check()) {
        _debug("No players in environment, stopping heartbeat = %O", this_object());
        set_heart_beat(0);
    }
}

int player_check() {
    object env ;

    if((env = environment()) && sizeof(present_players(env))) {
        return 1;
    }

    return 0;
}

int is_npc() { return 1; }
int is_pc() { return 0; }
