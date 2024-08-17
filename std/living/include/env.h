#ifndef __ENV_H__
#define __ENV_H__

int init_env() ;
int set_env(string var_name, string var_value) ;
varargs mixed query_env(string var_name, mixed def) ;
mapping list_env() ;
int set_pref(string pref_name, string pref_value) ;
varargs string query_pref(string pref_name, string def) ;
mapping list_pref() ;

#endif // __ENV_H__
