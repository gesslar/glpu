#ifndef __OBJECT_H__
#define __OBJECT_H__

// Function declarations for object handling
int set_name(string str);
string query_name();
string query_proper_name();
void set_proper_name(string str);

varargs void reset(mixed args...);
int remove();
string set_proper_name(string str);
string find_path(string path);
void set_virtual_master(string str);
string query_virtual_master();
int add_destruct(function f);
int remove_destruct(function f);
void process_destruct() ;

#endif // __OBJECT_H__
