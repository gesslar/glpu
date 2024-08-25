#ifndef __DESCRTIPTION_H__
#define __DESCRTIPTION_H__

int set_long(mixed str) ;
int set_short(mixed str) ;
string *query_extra_longs() ;
string *query_extra_shorts() ;
string query_extra_long(string id) ;
string query_extra_short(string id) ;
string query_long() ;
string query_short() ;
void add_extra_long(string id, mixed str) ;
void add_extra_short(string id, mixed str) ;
void remove_extra_long(string id) ;
void remove_extra_short(string id) ;

#endif // __DESCRTIPTION_H__
