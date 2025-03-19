#ifndef __BODY_H__
#define __BODY_H__

void rehash_capacity();
void die();
varargs int move_living(mixed dest, string dir, string depart_message, string arrive_message);
int query_log_level();
string *query_body_slots();
string *query_weapon_slots();
void set_su_body(object source);
object query_su_body();
void clear_su_body();

#endif // __BODY_H__
