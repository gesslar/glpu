#ifndef __CONTENTS_H__
#define __CONTENTS_H__

void rehash_contents();
void rehash_capacity();
void rehash_volume();
void clean_contents();
void set_max_capacity(int x);
void adjust_max_capacity(int x);
void set_max_volume(int x);
void adjust_max_volume(int x);
int query_max_capacity();
int query_max_volume();
int query_capacity();
int query_volume();
int adjust_capacity(int x);
int adjust_volume(int x);
int can_hold_object(object ob) ;
int can_hold_bulk(int bulk) ;
int can_hold_mass(int mass) ;

#endif // __CONTENTS_H__
