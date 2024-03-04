#include "/adm/obj/simul_efun.h"
#include <daemons.h>

//overrides.c

//Tacitus @ LPUniversity
//08-APR-05
//Simul-efuns

//Last edited on December 21st, 2005 by Tacitus

int exec( object to, object from ) {
    if(is_member(query_privs(previous_object()), "admin")) return efun::exec(to, from);
    else return 0;
}

void shutdown( int how ) {
    if(file_name(previous_object()) == "/adm/daemons/shutdown_d") efun::shutdown(how);
}

void set_privs(object ob, string privs) {
    string name;

    if(is_member(query_privs(previous_object()), "admin") || ob = master()) efun::set_privs(ob, privs);
    sscanf(file_name(ob), "/home/%*s/%s/%*s", name);
    if(name == privs) efun::set_privs(ob, privs);
}

void write(string msg) {
    if(this_player()) message("write", msg + "", this_player());
    else debug_message(msg) ;
}

varargs void say(string msg, mixed exclude) {
    object me;

    if(this_player()) me = this_player();
    else me = previous_object();

    if(objectp(exclude)) exclude = ({ me, exclude });
    else if(pointerp(exclude)) exclude += ({ me });
    else if(!exclude) exclude = ({ me });
    message("say", msg, environment(me), exclude);
}

varargs void shout(string msg, mixed exclude) {
    if(objectp(exclude))
        exclude = ({ exclude });
    else if(!pointerp(exclude))
        exclude = ({ });
    if(this_player())
        exclude += ({ this_player() });
    message("shout", msg, users(), exclude);
}

varargs void tell_object(mixed ob, mixed msg, mixed type) {
    if(!type) type = "tell_object" ;
    message(type, msg + "", ob);
}

varargs void tell_room(mixed room, string msg, mixed exclude) {
    if(!exclude || exclude==0) {
        message ("tell_room",msg,room) ;
    } else {
        message("tell_room", msg, room, exclude);
    }
}

varargs int userp(object ob) {
    if(!ob) ob = previous_object() ;
    return efun::userp(ob);
}

varargs string query_num(int x, int many) {
    string sign;

    if(nullp(x))
        error ("Too few arguments to 'query_num'.") ;

    if(!intp(x))
        error ("Bad argument 1 to 'query_num'.") ;

    sign = x < 0 ? "negative " : "" ;
    x = abs(x) ;

    return sign + efun::query_num(x, many) ;
}

string ctime(int x) {
    string fmt ;

    if(nullp(x)) x = time() ;

    fmt = "%Y-%m-%d %H:%M:%S" ;
    return strftime(fmt, x) ;
}

object this_body() {
    return this_player();
}

object this_user() {
    return this_player()->query_user();
}
