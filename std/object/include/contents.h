#ifndef __CONTENTS_H__
#define __CONTENTS_H__

void rehash_contents();
void rehash_capacity();
void rehash_volume();
void clean_contents();
void set_max_capacity(int x);
void set_max_volume(int x);
int query_max_capacity();
int query_max_volume();
int query_capacity();
int query_volume();
int add_capacity(int x);
int add_volume(int x);
int remove_capacity(int x);
int remove_volume(int x);
int can_hold_object(object ob) ;
int can_hold_bulk(int bulk) ;
int can_hold_mass(int mass) ;

#endif // __CONTENTS_H__
