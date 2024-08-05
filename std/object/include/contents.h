#ifndef __CONTENTS_H__
#define __CONTENTS_H__

void rehash_contents();
void rehash_capacity();
void set_max_capacity(int x);
void adjust_max_capacity(int x);
int query_max_capacity();
int query_capacity();
int adjust_capacity(int x);
int can_hold_object(object ob) ;
int can_hold_mass(int mass) ;

#endif // __CONTENTS_H__
