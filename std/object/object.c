/* object.c

 Tacitus @ LPUniversity
 04-APR-05
 Base Object

*/

/* Last updated July 14th, 2006 by Tacitus */

#include <config.h>

inherit STD_OB_E;
inherit M_SETUP ;
inherit __DIR__ "id" ;
inherit __DIR__ "description" ;

private string name, short, long;

void remove();
int move(mixed dest);
int moveAllowed(mixed dest);
int set_name(string str);
string query_name();
string query_cap_name();
int can_receive(object ob);
int can_release(object ob);

void create() {
    init_ob() ;
    setup_chain() ;
}

void event_remove(object prev) {
    load_object(VOID_OB);

    foreach(object ob in all_inventory()) {
        if(interactive(ob)) {
            tell_object(ob, "You watch as the environment around you disappears.\n");
            ob->move(VOID_OB);
        }
    }

    efun::destruct(this_object());
}

int move(mixed dest) {
    int result ;

    result = moveAllowed(dest) ;
    if(!result) return result ;

    move_object(dest);
    return 1;
}

int moveAllowed(mixed dest) {
    object ob;

    if(stringp(dest)) ob = load_object(dest);
    else if(objectp(dest)) ob = dest;

    if(!objectp(ob))
        return 0;

    if(!ob->can_receive(this_object()))
        return 0;

    if(environment())
        if(!environment()->can_release(this_object()))
            return 0;

    return 1;
}


int set_name(string str) {
    if(interactive(this_object()) && !isMember(query_privs(previous_object()), "admin")
        && previous_object() != this_object()) return 0;
    else {
        name = str;
        set("name", str);
        return 1;
    }
}

string query_name() {
    if(!name) return query("name");
    return name;
}

string query_cap_name() {
    if(!name && !stringp(query("name"))) return "";
    else if(!name) return capitalize(query("name"));
    else return capitalize(name);
}

string query_short() {
    if(!short) {
        if(!query("short")) return name;
        return query("short");
    }
    return short;
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
