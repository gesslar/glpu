#ifndef __SIMUL_EFUN_H__
#define __SIMUL_EFUN_H__

// File: description
string get_short(object ob) ;
string get_long(object ob) ;

// File: directory
void mkdirs(string path) ;

// File: exists
int directory_exists(string dirname) ;
int file_exists(string file) ;
int user_exists(string user) ;

// File: file
string file_owner(string file) ;
int file_lines(string file) ;
int tail(string file) ;
int log_file(string file, string str) ;
string *explode_file(string file) ;

// File: object
int getoid(object ob) ;

// File: resolve_path
string resolve_path(string Current, string Next) ;

// File: security
int isMember(string user, string group) ;
int adminp(mixed user) ;
int devp(mixed user) ;
int wizardp(mixed user) ;

// File: socket
string dump_socket_status() ;

// File: system
string mud_name() ;
string lib_name() ;
string baselib_name() ;
string lib_version() ;
string baselib_version() ;
string open_status() ;
string admin_email() ;
int port() ;
string driver_version() ;
string arch() ;

// File: user
string user_data_file(string name) ;
string user_mob_data(string name) ;
string user_data_directory(string priv) ;
string user_core_data_directory(string priv) ;
string user_path(string name) ;

// File: valids
int valid_function(mixed f) ;

#endif // __SIMUL_EFUN_H__
