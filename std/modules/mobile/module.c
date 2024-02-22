// /std/modules/mobile/module.c
// The sort of module that can be added to a mobile object.
//
// Created:     2024/02/21: Gesslar
// Last Change: 2024/02/21: Gesslar
//
// 2024/02/21: Gesslar - Created

inherit STD_OBJECT ;

protected nosave object owner;
protected nosave string module_name;

private varargs int start_module(mixed args...) ;
private void stop_module() ;

void mudlib_setup() {
    module_name = "module" ;
}

varargs void attach(object ob, mixed args...) {
    owner = ob ;
    owner->attach_module(this_object(), args...) ;
    return start_module(args...) ;
}

void detach() {
    stop_module() ;
    owner->detach_module(this_object()) ;
    owner = 0 ;
}

object query_owner() {
    return owner ;
}

int remove() {
    if(objectp(owner)) {
        detach() ;
    }

    return ::remove() ;
}

int request_clean_up() {
    if(!clonep()) return 1 ;
    if(!objectp(owner)) return 1 ;
    return 0 ;
}
