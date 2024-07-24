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
inherit __DIR__ "init" ;
inherit __DIR__ "persist" ;
inherit __DIR__ "heartbeat" ;

inherit M_CLEAN ;
inherit M_SETUP ;
inherit M_MESSAGING ;

private string proper_name, short, long;
private nosave string name ;
private nosave mapping spawn_info = ([]) ;

int move(mixed dest);
int allow_move(mixed dest);
int set_name(string str);
string query_name();
string query_proper_name() ;
void set_proper_name(string str);

void set_spawn_info(mapping info);
void add_spawn_info(string key, mixed value);
mixed query_spawn_info(string key);
mapping query_all_spawn_info();

int can_receive(object ob);
int can_release(object ob);

void on_remove(object env) {}

// Private so only driver can call it.
private void create() {
    init_ob() ;
    if(!proper_name) {
            if(name)
            set_proper_name(name) ;
        else
            name = null ;
    }

    reset() ;
}

void reset() {
    setup_chain() ;
    call_if(this_object(), "reset_objects") ;
}

int remove() {
    object env = environment() ;
    object *inv ;

    if(env && !env->is_room()) {
        environment()->add_capacity(-query_mass());
        environment()->add_volume(-query_bulk());
    }

    on_remove(environment()) ;

    inv = all_inventory() ;
    inv = filter(inv, (: !userp($1) :)) ;

    catch(inv->remove()) ;

    destruct() ;
    return 1 ;
}

int move(mixed dest) {
    int result ;
    object prev ;

    result = allow_move(dest) ;
    if(!(result & MOVE_OK)) return result ;

    if(result & MOVE_ALREADY_THERE) return MOVE_OK | MOVE_ALREADY_THERE ;

    prev = environment() ;
    if(prev && !prev->is_room()) {
        prev->add_capacity(query_mass());
        prev->add_volume(query_bulk());
    }

    if(!dest->is_room()) {
        dest->add_capacity(-query_mass());
        dest->add_volume(-query_bulk());
    }

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

    if(!objectp(ob))
        return MOVE_NO_DEST;

    if(environment() == ob)
        return MOVE_ALREADY_THERE ;

    if(!ob->can_receive(this_object()))
        return MOVE_NOT_ALLOWED;

    if(mud_config("USE_MASS"))
        if(!ob->query_ignore_mass())
            if(ob->query_capacity() < query_mass())
                return MOVE_TOO_HEAVY ;

    if(mud_config("USE_BULK"))
        if(!ob->query_ignore_bulk())
            if(ob->query_volume() < query_bulk())
                return MOVE_NO_ROOM ;

    if(environment())
        if(!environment()->can_release(this_object()))
            return 0;

    return MOVE_OK ;
}

void set_spawn_info(mapping info) {
    spawn_info = info ;
}

void add_spawn_info(string key, mixed value) {
    spawn_info[key] = value ;
}

mixed query_spawn_info(string key) {
    return spawn_info[key] ;
}

mapping query_all_spawn_info() {
    return copy(spawn_info) ;
}

void set_proper_name(string str) {
    if(interactive(this_object()) && !is_member(query_privs(previous_object()), "admin")
        && previous_object() != this_object()) return 0;
    proper_name = lower_case(str);
    if(living())
        set_living_name(proper_name);
}

string query_proper_name() {
    return proper_name ;
}

int set_name(string str) {
    if(!str)
        return 0;

    name = capitalize(lower_case(str)) ;

    if(!proper_name) set_proper_name(str);
    rehash_ids() ;
    return 1;
}

string query_name() {
    return name ;
}

int can_receive(object ob) {
    return 1;
}

int can_release(object ob) {
    return 1;
}

void register_crash() {
    CRASH_D->register_crash_object(this_object()) ;
}

void unregister_crash() {
    CRASH_D->unregister_crash_object(this_object()) ;
}
