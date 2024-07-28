#ifndef __BODY_H__
#define __BODY_H__

/* Connection functions */
void enter_world() ;
void exit_world() ;
void reconnect() ;

/* User object functions */
varargs int move(mixed ob, int flag) ;
void restore_user() ;
void save_user() ;
object query_user() ;
int set_user(object ob) ;

/* User environmental variable functions */
int set_env(string var_name, string var_value) ;
mixed query_env(string var_name, mixed def) ;
mapping list_env() ;

/* User path functions */
string *query_path() ;
void add_path(string str) ;
void rem_path(string str) ;

/* Communication & Interaction functions */
void catch_tell(string message) ;
void receive_message(string type, string msg) ;
string process_input(string arg) ;
int command_hook(string arg) ;
private nomask int evaluate_result(mixed result) ;
mixed* query_commands() ;
int force_me(string cmd) ;

/* Module functions */
object add_module(string module) ;
void remove_module(string module) ;
object get_module(string module) ;
varargs mixed module(string module, string func, mixed args...) ;

/* Misc functions */
void write_prompt() ;

/* Functions */
void create() ;
void query_body_slots() ;
void setup_body() ;
void init_capacity() ;
void net_dead() ;
void heart_beat() ;
void die() ;
void event_remove(object prev) ;
void remove_all_modules() ;
void move_or_destruct(object ob) ;
void set_gmcp_client(mapping data) ;
mapping query_gmcp_client() ;
void set_gmcp_supports(mapping data) ;
mapping query_gmcp_supports() ;
int query_gmcp_supported(string fullname) ;
mixed query_environ(string key) ;
void clear_environ() ;
mapping query_all_environ() ;
void set_environ_option(string key, mixed value) ;
void receive_environ(string var, mixed value) ;
void set_environ(mapping data) ;
int has_screenreader() ;
int query_log_level() ;
int supports_unicode() ;
void on_crash(mixed arg...) ;
void mudlib_unsetup() ;
int is_pc() ;
int is_npc() ;

#endif // __BODY_H__
