#ifndef __BODY_H__
#define __BODY_H__

string *query_body_slots();
string *query_all_commands();
void init_capacity();
void die();
object query_user();
int set_user(object ob);
int set_env(string var_name, string var_value);
varargs mixed query_env(string var_name, mixed def);
mapping list_env();
string *query_path();
void add_path(string str);
void rem_path(string str);
nomask varargs string *query_command_history(int index, int range);
int command_hook(string arg);
private nomask int evaluate_result(mixed result);
varargs int move_living(mixed dest, string dir, string depart_message, string arrive_message);
mixed* query_commands();
int force_me(string cmd);
int query_log_level();

#endif // __BODY_H__
