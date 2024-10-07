#ifndef __COMMAND_H__
#define __COMMAND_H__

// Function declarations for command handling
void add_command(mixed command, mixed action) ;
void remove_command(mixed command) ;
void remove_command_all(mixed action) ;
mixed query_command(string command) ;
mapping query_commands() ;
string *query_matching_commands(string command) ;
void init_commands() ;
mixed evaluate_command(object tp, string command, string arg) ;

#endif // __COMMAND_H__
