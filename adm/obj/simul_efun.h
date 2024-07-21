#ifndef __SIMUL_EFUN_H__
#define __SIMUL_EFUN_H__

// File: arrays
mixed *distinct_array(mixed *arr) ;
varargs mixed *remove_array_element(mixed *arr, int start, int end) ;
varargs mixed *splice(mixed *arr, int start, int deleteCount, mixed *itemsToAdd) ;
mixed *reverse_array(mixed *arr) ;

// File: base64
string base64_encode(mixed source_str) ;
string base64_decode(string source) ;

// File: data
varargs mixed data_value(string file, string key, mixed def) ;
varargs void data_write(string file, string key, mixed data...) ;
varargs int data_inc(string file, string key, int inc) ;
int data_del(string file, string key) ;

// File: description
string get_short(object ob) ;
string get_long(object ob) ;

// File: directory
int assure_dir(string path) ;
string query_directory(object ob) ;

// File: english
string cap_words(string str) ;
varargs string cap_significant_words(string str, int title) ;
string possessive_noun(mixed ob) ;
string possessive_pronoun(mixed ob) ;
string possessive(mixed ob) ;
string reflexive(mixed ob) ;
string objective(mixed ob) ;
string subjective(mixed ob) ;

// File: exists
int directory_exists(string dirname) ;
int file_exists(string file) ;
int cfile_exists(string file) ;
int ofile_exists(string file) ;
int user_exists(string user) ;

// File: file
int file_lines(string file) ;
int log_file(string file, string str) ;
string *explode_file(string file) ;
varargs void implode_file(string file, string *lines, int overwrite) ;
string file_owner(string file) ;
string query_file_name(object ob) ;
varargs string tail(string path, int line_count) ;
void assure_file(string file) ;
varargs string temp_file(mixed arg) ;
string *dir_file(string path) ;
int touch(string file) ;

// File: function
int valid_function(mixed f) ;
mixed *assemble_call_back(mixed arg...) ;
mixed call_back(mixed *cb, mixed arg...) ;
varargs string call_trace(int colour) ;
mixed call_if(mixed ob, string func, mixed arg) ;

// File: grammar
string int_string (int num) ;
string ordinal (int n) ;

// File: identify
string identify(mixed arg) ;

// File: json
mixed json_decode(string str) ;
mapping json_encode(mixed arg) ;

// File: mappings
string pretty_map(mapping map);
mixed element_of_weighted(mapping m) ;

// File: messaging
varargs void tell_up(object ob, string str, int msg_type, mixed exclude) ;
varargs void tell_down(object ob, string str, int msg_type, mixed exclude) ;
varargs void tell_all(object ob, string str, int msg_type, mixed exclude) ;
varargs void tell_direct(object ob, string str, int msg_type, mixed exclude) ;
varargs void tell(object ob, string str, int msg_type, mixed exclude) ;

// File: numbers
float percent_of(float a, float b) ;
float percent(float a, float b) ;
float range(float min, float max, float val) ;
varargs float remainder(mixed a, mixed b) ;
int sum(mixed *arr) ;

// File: object
int getoid(object ob) ;
varargs mixed get_objects( string str, object player, int no_arr ) ;
varargs object find_ob(mixed ob, mixed cont) ;
varargs object get_object( string str, object player ) ;
object top_environment(object ob) ;
object *present_livings(object room) ;
object *present_players(object room) ;
object get_living(string name, object room) ;
object *get_livings(mixed names, object room) ;
object get_player(string name, object room) ;
object *get_players(mixed names, object room) ;
object this_body() ;

// File: prompt
void prompt_password(object user, int attempts, mixed *cb) ;

// File: resolve_path
string resolve_path(string Current, string Next) ;

// File: save
string object_save_directory(object ob) ;
string object_save_file(object ob) ;
string assure_object_save_dir(object ob) ;

// File: security
int is_member(string user, string group) ;
int adminp(mixed user) ;
int devp(mixed user) ;
int wizardp(mixed user) ;

// File: socket
string dump_socket_status() ;

// File: string
string append(string source, string to_append) ;
string no_ansi(string str) ;
string prepend(string source, string to_prepend) ;
varargs string chop(string str, string sub, int dir) ;
varargs string extract(string str, int from, int to) ;
varargs string simple_list(mixed *arr, string conj) ;
varargs string substr(string str, string sub, int reverse) ;
varargs mixed from_string(string str, int flag) ;
string stringify(mixed val) ;
string add_commas(mixed number) ;
string reverse_string(string str) ;
string reverse_strsrch(string str, string sub, int start) ;

// File: system
int port() ;
mixed mud_config(string str) ;
string admin_email() ;
string arch() ;
string baselib_name() ;
string baselib_version() ;
string driver_version() ;
string lib_name() ;
string lib_version() ;
string mud_name() ;
string open_status() ;
varargs void _debug(string str, mixed args...) ;
string log_dir() ;
string tmp_dir() ;
varargs int _ok(mixed args...) ;
varargs int _info(mixed args...) ;
varargs int _error(mixed args...) ;
varargs int _question(mixed args...) ;
varargs int _warn(mixed args...) ;

// File: time
varargs string ldate(int x, int brief) ;
varargs string ltime(int x, int brief) ;
int time_ms() ;
float time_frac(int nanoseconds) ;

// File: user
string user_data_file(string name) ;
string user_mob_data(string name) ;
string user_data_directory(string priv) ;
string user_core_data_directory(string priv) ;
string user_path(string name) ;

// File: util
string generate_uuid() ;
int of(mixed needle, mixed haystack) ;

#endif // __SIMUL_EFUN_H__
