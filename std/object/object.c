/* object.c

 Tacitus @ LPUniversity
 04-APR-05
 Base Object

*/

/* Last updated July 14th, 2006 by Tacitus */

#include <object.h>

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

private nomask nosave function *destruct_functions = ({}) ;
private nomask nosave function *reset_functions = ({}) ;
private string proper_name, short, long;
private nosave string name ;
protected nosave mixed *create_args = ({}) ;
private nosave string virtual_master = 0;

// Private so only driver can call it.
private varargs void create(mixed args...) {
    set_notify_destruct(1) ;
    init_ob() ;

    create_args = args ;
    if(!proper_name) {
            if(name)
            set_proper_name(name) ;
        else
            name = null ;
    }

    setup_chain(create_args...) ;
    reset() ;
}

void virtual_start() {
    virtual_setup_chain(create_args...) ;
    reset() ;
}

varargs void reset() {
    call_if(this_object(), "reset_objects") ;
    process_reset() ;
}

int remove() {
    object ob, next, env = environment() ;

    catch(call_if(this_object(), "on_remove", environment())) ;

    if(!env) {
        ob = first_inventory() ;
        while(ob) {
            next = next_inventory(ob) ;
            ob->remove() ;
            if(objectp(ob))
                destruct(ob) ;
            ob = next ;
        }
    } else {
        ob = first_inventory() ;
        while(ob) {
            next = next_inventory(ob) ;
            if(ob->move(env)) {
                ob->remove() ;
                if(objectp(ob))
                    destruct(ob) ;
            }
            ob = next ;
        }
    }

    if(sizeof(all_inventory()))
        filter(all_inventory(), (: destruct :)) ;

    if(mud_config("USE_MASS")) {
        if(env && !env->ignore_mass()) {
            int mass = query_mass() ;
            printf("Removing %d mass from %O\n", mass, env) ;
            printf("  Old capacity: %d\n", env->query_capacity()) ;
            env->adjust_capacity(-mass) ;
            printf("  New capacity: %d\n", env->query_capacity()) ;
        }
    }

    destruct() ;
    return 1 ;
}

string set_proper_name(string str) {
    if(interactive(this_object()) && !is_member(query_privs(previous_object()), "admin")
        && previous_object() != this_object()) return 0;

    proper_name = lower_case(str);

    if(living())
        set_living_name(proper_name);

    return proper_name ;
}

string query_proper_name() {
    return proper_name ;
}

string set_name(string str) {
    string result ;

    if(!str)
        return 0;

    name = capitalize(lower_case(str)) ;

    result = set_proper_name(name) ;
    if(!result)
        return 0;

    rehash_ids() ;

    return result ;
}

string query_name() {
    return name ;
}

string find_path(string path) {
    return resolve_path(query_directory(), path) ;
}

void set_virtual_master(string str) {
    virtual_master = str;
}

string query_virtual_master() {
    return virtual_master;
}

void on_destruct() {
    object env = environment() ;

    if(env && !env->is_room()) {
        env->adjust_capacity(query_mass());
    }

    process_destruct() ;
    unsetup_chain() ;
}

void add_reset(function f) {
    if(valid_function(f) && !of(f, reset_functions)) {
        reset_functions += ({ f }) ;
    }
}

void remove_reset(function f) {
    if(of(f, reset_functions)) {
        reset_functions -= ({ f }) ;
    }
}

void process_reset() {
    foreach(function f in reset_functions) {
        catch(evaluate(f)) ;
    }
}

int add_destruct(function f) {
    if(valid_function(f) && !of(f, destruct_functions)) {
        destruct_functions += ({ f }) ;
        return 1 ;
    }

    return 0 ;
}

int remove_destruct(function f) {
    if(of(f, destruct_functions)) {
        destruct_functions -= ({ f }) ;
        return 1 ;
    }

    return 0 ;
}

void process_destruct() {
    foreach(function f in destruct_functions) {
        catch(evaluate(f)) ;
    }
}
