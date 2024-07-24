/* user.c

 Tacitus @ LPUniversity
 04-APR-05
 User Object

*/

/* Last edited on July 17th, 2006 by Tacitus */

#include <config.h>
#include <origin.h>
#include <clean.h>

inherit STD_OBJECT;
inherit M_LOG ;

private string password, body_path;

private nosave object body;

int save_user();

void create() {
    if(origin() != ORIGIN_DRIVER) return;
    if(!body_path) body_path = STD_BODY ;
    set_log_level(0) ;
}

nomask void net_dead() {
    if(origin() != ORIGIN_DRIVER) return;

    set("last_login", time());
    save_user();
    remove() ;
}

nomask void restore_user() {
    if(!is_member(query_privs(previous_object() ? previous_object() : this_body()), "admin") && this_body() != this_object()) return 0;
    if(is_member(query_privs(previous_object()), "admin") || query_privs(previous_object()) == query_privs(this_body())) restore_object(user_data_file(query_privs()));
    set_log_prefix(sprintf("(%O)", this_object())) ;
}

nomask void save_user() {
    int result ;

    if(!is_member(query_privs(previous_object() ? previous_object() : this_body()), "admin") &&
       this_body() != this_object() &&
       origin() != ORIGIN_LOCAL) {
        return 0;
    } else {
        result = save_object(user_data_file(query_privs()));
    }
}

nomask int set_password(string str) {
    if(adminp(query_privs(previous_object())) || this_body() == body) {
        password = crypt(str, 0);
    } else {
        return 0;
    }

    if(ofile_exists(user_data_file(query_privs())))
        save_user() ;

    return 1;
}

nomask mixed query_password() {
    if(adminp(query_privs(previous_object())) || this_body() == body) return password;
    else return "Error [user]: Permission Denied.\n";
}

nomask int set_body_path(string file) {
    if(!adminp(query_privs(previous_object())) && this_body() != body) return 0;
    if(!file_exists(file)) return 0;
    body_path = file;
    return 1;
}

nomask string query_body_path() {
    return body_path;
}

nomask int set_body(object ob) {
    if(!adminp(query_privs(previous_object())) && this_body() != body) return 0;
    ob->set_user(this_object());
    body = ob;
}

nomask object query_body() {
    return body;
}

void setup() {
    return;
}

void catch_tell(string message) {
    receive(message);
}

int clean_up(int refs) {
    if(body) {
        return CLEAN_LATER ;
    }

    return ::clean_up(refs) ;
}

int query_log_level() {
    return query_body()->query_log_level() ;
}

void receive_environ(string key, mixed data) {
    _log("%s => %O", key, data) ;
}
