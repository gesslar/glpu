/* object.c

 Tacitus @ LPUniversity
 04-APR-05
 Base Object

*/

/* Last updated July 14th, 2006 by Tacitus */

#include <config.h>

inherit STD_OB_E;

inherit __DIR__ "id" ;
inherit __DIR__ "description" ;
inherit __DIR__ "stats" ;
inherit __DIR__ "contents" ;
inherit __DIR__ "weight" ;

inherit M_CLEAN ;
inherit M_SETUP ;
inherit M_MESSAGING ;

private string proper_name, name, cap_name, short, long;

int move(mixed dest);
int allow_move(mixed dest);
int set_name(string str);
string query_name();
string query_cap_name();
string query_proper_name() ;
void set_proper_name(string str);
string name() ;
string set_cap_name(string str);

int can_receive(object ob);
int can_release(object ob);

// Private so only driver can call it.
private void create() {
    init_ob() ;
    setup_chain() ;
    if(!proper_name) {
        set_proper_name(name) ;
        name = UNDEFINED ;
    }
}

int remove() {
    if(environment()) {
        environment()->add_capacity(-query_mass());
        environment()->add_volume(-query_bulk());
    }

    destruct() ;
    return 1 ;
}

int move(mixed dest) {
    int result ;
    object prev ;

    result = allow_move(dest) ;
    if(!(result & MOVE_OK)) return result ;

    prev = environment() ;
    if(prev) {
        prev->add_capacity(-query_mass());
        prev->add_volume(-query_bulk());
    }

    dest->add_capacity(query_mass());
    dest->add_volume(query_bulk());

    move_object(dest);
    event(this_object(), "moved", prev) ;
    if(prev && this_object()) event(prev, "released", environment()) ;
    if(this_object()) event(environment(), "received", prev) ;

    if(this_object()) return MOVE_OK ;
    else return MOVE_OK | MOVE_DESTRUCTED ;
}

int allow_move(mixed dest) {
    object ob;

    if(stringp(dest)) catch(ob = load_object(dest)) ;
    else if(objectp(dest)) ob = dest;

    if(!objectp(ob)) {
        return MOVE_NO_DEST;
    }

    if(!ob->can_receive(this_object())) {
        return MOVE_NOT_ALLOWED;
    }

    if(mud_config("USE_MASS")) {
        if(!ob->query_ignore_mass()) {
            if(ob->query_capacity() < query_mass()) {
                return MOVE_TOO_HEAVY ;
            }
        }
    }

    if(mud_config("USE_BULK")) {
        if(!ob->query_ignore_bulk()) {
            if(ob->query_volume() < query_bulk()) {
                return MOVE_NO_ROOM ;
            }
        }
    }

    if(environment())
        if(!environment()->can_release(this_object()))
            return 0;

    return MOVE_OK ;
}

void set_proper_name(string str) {
    if(interactive(this_object()) && !is_member(query_privs(previous_object()), "admin")
        && previous_object() != this_object()) return 0;
    proper_name = str;
}

string query_proper_name() {
    return proper_name;
}

string name() {
    return query_proper_name() ;
}

int set_name(string str) {
    name = str;
    set_cap_name(str) ;
    if(!proper_name) set_proper_name(str);
    rehash_ids() ;
}

void set_cap_name(string str) {
    cap_name = capitalize(str);
    rehash_ids() ;
}

string query_name() {
    return name;
}

string query_cap_name() {
    if(!name) return "";
    else return capitalize(name);
}

string query_short() {
    string result = ::query_short();
    if(!result) result = query_name() ;

    return result ;
}

int set_long(string str) {
    long = str;
    set("long", str);
}

string query_long() {
    if(!long) return query("long");
    return long;
}

int can_receive(object ob) {
    return 1;
}

int can_release(object ob) {
    return 1;
}
