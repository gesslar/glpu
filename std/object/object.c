/* object.c

 Tacitus @ LPUniversity
 04-APR-05
 Base Object

*/

/* Last updated July 14th, 2006 by Tacitus */

inherit STD_OB_E;

inherit __DIR__ "contents" ;
inherit __DIR__ "description" ;
inherit __DIR__ "heartbeat" ;
inherit __DIR__ "id" ;
inherit __DIR__ "init" ;
inherit __DIR__ "persist" ;
inherit __DIR__ "setup" ;
inherit __DIR__ "stats" ;
inherit __DIR__ "weight" ;

inherit M_CLEAN ;
inherit M_MESSAGING ;

private string proper_name, short, long;
private nosave string name ;
protected nosave mixed *create_args = ({}) ;

int set_name(string str);
string query_name();
string query_proper_name() ;
void set_proper_name(string str);

varargs void reset(mixed args...);

// Private so only driver can call it.
protected varargs void create(mixed args...) {
    set_notify_destruct(1) ;
    init_ob() ;

    create_args = args ;
    if(!proper_name) {
            if(name)
            set_proper_name(name) ;
        else
            name = null ;
    }
}

varargs void reset() {
    call_if(this_object(), "reset_objects") ;
}

int remove() {
    object env = environment() ;
    object *inv ;

    catch(call_if(this_object(), "on_remove", environment())) ;

    inv = all_inventory() ;
    inv = filter(inv, (: !userp($1) :)) ;

    catch(inv->remove()) ;

    destruct() ;
    return 1 ;
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

void register_crash() {
    CRASH_D->register_crash_object(this_object()) ;
}

void unregister_crash() {
    CRASH_D->unregister_crash_object(this_object()) ;
}

string find_path(string path) {
    return resolve_path(query_directory(), path) ;
}
