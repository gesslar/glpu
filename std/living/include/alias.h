#ifndef __ALIAS_H__
#define __ALIAS_H__

void add_alias(string verb, string cmd);
int remove_alias(string verb);
mapping get_aliases(int all);
string compute_alias(string al, string args);
string alias_parse(string verb, string args);

#endif // __ALIAS_H__
