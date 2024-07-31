#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

void init_attributes();
int set_attribute(string key, int value);
int query_attribute(string key);
int modify_attribute(string key, int value);
mapping query_attributes();

#endif // __ATTRIBUTES_H__
