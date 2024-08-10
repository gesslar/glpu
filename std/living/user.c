/* user.c

 Tacitus @ LPUniversity
 04-APR-05
 User Object

*/

/* Last edited on July 17th, 2006 by Tacitus */

#include <origin.h>
#include <commands.h>

inherit STD_OBJECT;
inherit M_LOG ;

private string account, body_path;

private nosave object body;
private nosave object su_body ;

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
    if(!valid_account(query_privs()))
        ACCOUNT_D->create_account(query_privs(), crypt(str, 0));
    else
        ACCOUNT_D->write_account(query_privs(), "password", crypt(str, 0));

    return 1;
}

nomask mixed query_password() {
    return ACCOUNT_D->read_account(query_privs(), "password");
}

string set_account(string str) {
    if(!valid_account(str)) return 0;
    account = str;
    save_user();
    return account;
}

string query_account() {
    return account;
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
