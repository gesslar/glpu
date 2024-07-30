/* user.c

 Tacitus @ LPUniversity
 04-APR-05
 User Object

*/

/* Last edited on July 17th, 2006 by Tacitus */

#include <origin.h>
#include <clean.h>
#include <commands.h>

inherit STD_OBJECT;
inherit M_LOG ;

private string password, body_path;

private nosave object body;
private nosave object su_body ;
private nosave mapping gmcp_data = ([ ]);
private nosave mapping environ_data = ([]) ;

int save_user();

void create() {
    if(origin() != ORIGIN_DRIVER) return;
    /*if(!body_path)*/ body_path = STD_PLAYER ;
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
    if(adminp(query_privs(previous_object())) ||
       this_body() == body ||
       base_name(previous_object()) == "/std/modules/gmcp/Char")
       return password;

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
    object po = previous_object() ;

    if(!adminp(query_privs(po)) && this_body() != body && base_name(po) != CMD_SU &&
       origin() != ORIGIN_LOCAL)
        return 0;

    ob->set_user(this_object());
    body = ob;
    body->clear_environ() ;
    body->set_environ(environ_data) ;
    body->clear_gmcp_data() ;
    body->set_gmcp_client(gmcp_data["client"]);
    body->set_gmcp_supports(gmcp_data["supports"]);

    return 1 ;
}

nomask object query_body() {
    return body;
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

void clear_gmcp_data() {
    gmcp_data = ([ ]);
}

void set_gmcp_data(mapping data) {
    if(base_name(previous_object()) != BODY_D)
        return;

    if(!mapp(data))
        return ;

    gmcp_data = data ;
}

void set_environ_data(mapping data) {
    if(base_name(previous_object()) != BODY_D)
        return;

    if(!mapp(data))
        return ;

    environ_data = data ;
}

void clear_environ_data() {
    environ_data = ([ ]) ;
}

int set_su_body(object source, object dest) {
    if(!objectp(source) || !objectp(dest))
        return 0;

    if(objectp(su_body))
        return 0;

    su_body = source ;
    return 1;
}

object query_su_body() {
    return su_body ;
}

int clear_su_body() {
    if(!objectp(su_body))
        return 0;

    su_body = 0 ;

    return 1;
}

object revert() {
    object ret ;

    if(!objectp(su_body))
        return 0 ;

    if(!objectp(body))
        return 0;

    if(!exec(su_body, body))
        return 0 ;

    if(!set_body(su_body))
        return 0 ;

    ret = su_body ;

    clear_su_body() ;

    return ret ;
}

int is_user() { return 1 ; }
