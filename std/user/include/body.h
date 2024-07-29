#ifndef __BODY_H__
#define __BODY_H__

string *query_all_commands();
void enter_world();
void exit_world();
void reconnect();
void die();
void restore_user();
int save_user();
object query_user();
int set_user(object ob);
int set_env(string var_name, string var_value);
varargs mixed query_env(string var_name, mixed def);
mapping list_env();
string *query_path();
void add_path(string str);
void rem_path(string str);
void catch_tell(string message);
void receive_message(string type, string msg);
string process_input(string arg);
nomask varargs string *query_command_history(int index, int range);
int command_hook(string arg);
private nomask int evaluate_result(mixed result);
varargs int move_living(mixed dest, string dir, string depart_message, string arrive_message);
mixed* query_commands();
int force_me(string cmd);
varargs object add_module(string module, mixed args...);
object query_module(string module);
void remove_module(string module);
object get_module(string module);
mapping query_modules();
varargs mixed module(string module, string func, mixed args...);
void remove_all_modules();
public mapping query_equipped();
public object equipped_on(string slot);
mixed can_equip(string slot, object ob);
int equip(string slot, object ob);
int unequip(string slot);
void move_or_destruct(object ob);
void clear_gmcp_data();
void set_gmcp_client(mapping data);
mapping query_gmcp_client();
void set_gmcp_supports(mapping data);
mapping query_gmcp_supports();
int query_gmcp_supported(string fullname);
mixed query_environ(string key);
void clear_environ_data();
mapping query_all_environ();
void set_environ_option(string key, mixed value);
void receive_environ(string var, mixed value);
void set_environ(mapping data);
int has_screenreader();
int query_log_level();
int supports_unicode();
int is_pc();
int is_npc();

#endif // __BODY_H__
