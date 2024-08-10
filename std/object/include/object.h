#ifndef __OBJECT_H__
#define __OBJECT_H__

// Function declarations for object handling
int set_name(string str);
string query_name();
string query_real_name();
void set_real_name(string str);

varargs void reset(mixed args...);
int remove();
string set_real_name(string str);
string find_path(string path);
void set_virtual_master(string str);
string query_virtual_master();
int add_destruct(function f);
int remove_destruct(function f);
void process_destruct() ;
void add_reset(function f) ;
void remove_reset(function f) ;
void process_reset() ;

#endif // __OBJECT_H__
