/* user.c

 Tacitus @ LPUniversity
 04-APR-05
 User Object

*/

/* Last edited on July 17th, 2006 by Tacitus */

#include <config.h>
#include <driver/origin.h>

inherit STD_OBJECT;

private string password, body_path;

private nosave object body;

int save_user();
void remove();

void create() {
    if(origin() != ORIGIN_DRIVER) return;
    if(!body_path) body_path = "/std/user/mobile";
}

nomask void net_dead() {
    if(origin() != ORIGIN_DRIVER) return;

    set("last_login", time());
    save_user();
    destruct(this_object());
}

nomask void restore_user() {
    if(!is_member(query_privs(previous_object() ? previous_object() : this_player()), "admin") && this_player() != this_object()) return 0;
    if(is_member(query_privs(previous_object()), "admin") || query_privs(previous_object()) == query_privs(this_player())) restore_object(user_data_file(query_privs(this_object())));
}

nomask void save_user() {
    int result ;

    if(!is_member(query_privs(previous_object() ? previous_object() : this_player()), "admin") &&
       this_player() != this_object() &&
       origin() != ORIGIN_LOCAL) {
        return 0;
    } else {
        result = save_object(user_data_file(query_privs(this_object())));
    }

}

nomask int set_password(string str) {
    if(adminp(query_privs(previous_object())) || this_player() == body) {
        password = crypt(str, 0);
    } else {
        return 0;
    }

    if(file_exists(user_data_file(query_privs(this_object())) + __SAVE_EXTENSION__))
        save_user() ;

    return 1;
}

nomask mixed query_password() {
    if(adminp(query_privs(previous_object())) || this_player() == body) return password;
    else return "Error [user]: Permission Denied.\n";
}

nomask int set_body_path(string file) {
    if(!adminp(query_privs(previous_object())) && this_player() != body) return 0;
    if(!file_exists(file)) return 0;
    body_path = file;
    return 1;
}

nomask string query_body_path() {
    return body_path;
}

nomask int set_body(object ob) {
    if(!adminp(query_privs(previous_object())) && this_player() != body) return 0;
    ob->set_link(this_object());
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
