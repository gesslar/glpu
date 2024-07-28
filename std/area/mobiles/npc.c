/* npc.c

 Tacitus @ LPUniversity
 28-JUNE-06
 Non-player character

*/

inherit STD_BODY;

void setup_body() {
    set_name("Generic NPC");
    set_living_name(lower_case(query_name()));
    set_ids(({lower_case(query_name()), query_name(), "npc", "generic npc", "NPC" }));
    if(!clonep()) set_heart_beat(0) ;
    set_long(capitalize(query_name()) + " is a generic NPC.");
    set_short(query_name());
}

void restore_user() {}
void save_user() {}

int force_me(string cmd) {
    return command(cmd);
}

int is_npc() { return 1; }
int is_pc() { return 0; }
