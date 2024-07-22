/* npc.c

 Tacitus @ LPUniversity
 28-JUNE-06
 Non-player character

*/

inherit STD_BODY;

void create() {
    path = ({ "/cmds/std/" });
    set("prevent_get", 1);
    if(!mapp(query("env_settings"))) set("env_settings", ([])) ;
    if(!query_env("cwd")) set_env("cwd", "/doc");
    if(!query_env("colour")) set_env("colour", "on");
    if(!query_env("news_client")) set_env("news_client", "/obj/mudlib/newsclients/std_newsclient.c");
    if(!query_env("auto_tune")) set_env("auto_tune", "local_net");
    enable_commands();
    setup();
    add_action("command_hook", "", 1);
}

// void setup_body() {
//     set_name("Generic NPC");
//     set_living_name(lower_case(name()));
//     set_ids(({name(), "npc", "generic npc", "NPC" }));
//     if(clonep()) set_heart_beat(1);
//     set_long(capitalize(name()) + " is a generic NPC.");
//     set_short(name());
// }

void restore_user() {}
void save_user() {}

int force_me(string cmd) {
    return command(cmd);
}

int is_npc() { return 1; }
int is_pc() { return 0; }
