#ifndef __ITEM_H__
#define __ITEM_H__

void set_spawn_info(mapping info) ;
void add_spawn_info(string key, mixed value) ;
mixed query_spawn_info(string key) ;
mapping query_all_spawn_info() ;
int move(mixed dest) ;
void allow_move(mixed dest) ;

#endif // __ITEM_H__
