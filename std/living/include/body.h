#ifndef __BODY_H__
#define __BODY_H__

string *query_all_commands() ;
void rehash_capacity() ;
void die() ;
string *query_path() ;
void add_path(string str) ;
void rem_path(string str) ;
nomask varargs string *query_command_history(int index, int range) ;
int command_hook(string arg) ;
private nomask int evaluate_result(mixed result) ;
varargs int move_living(mixed dest, string dir, string depart_message, string arrive_message) ;
mixed* query_commands() ;
int force_me(string cmd) ;
int query_log_level() ;
string *query_body_slots() ;
string *query_weapon_slots() ;
void set_su_body(object source) ;
object query_su_body() ;
void clear_su_body() ;

#endif // __BODY_H__
