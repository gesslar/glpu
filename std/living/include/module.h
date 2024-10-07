#ifndef __MODULE_H__
#define __MODULE_H__

varargs object add_module(string module, mixed args...) ;
object query_module(string module) ;
void remove_module(string module) ;
object get_module(string module) ;
mapping query_modules() ;
varargs mixed module(string module, string func, mixed args...) ;
void remove_all_modules() ;

#endif // __MODULE_H__
