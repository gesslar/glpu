#ifndef __ROOM_H__
#define __ROOM_H__


mapping gmcp_room_info(object who);
void set_room_size(int *size);
int *query_room_size();
string set_room_type(string type);
string set_room_subtype(string subtype);
string set_room_icon(string icon);
string query_room_type();
string query_room_subtype();
string query_room_icon();
int no_renew();
int is_room();

#endif // __ROOM_H__
