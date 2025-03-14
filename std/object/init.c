// /std/modules/init.c
// The standard module for using the init() apply
//
// Created:     2024/02/29: Gesslar
// Last Change: 2024/02/29: Gesslar
//
// 2024/02/29: Gesslar - Created

inherit __DIR__ "command";

private nosave mixed *inits = ({});

void init() {
    event(({this_object()}), "init", this_body());
}

void event_init(object prev, object tp) {
    int sz;

    sz = sizeof(inits);
    for(int i = 0; i < sz; i++) {
        if(valid_function(inits[i])) {
            (*inits[i])(tp);
        } else {
            catch(call_other(this_object(), inits[i], tp));
        }
    }
}

void add_init(mixed fun) {
    if(valid_function(fun) || stringp(fun)) {
        inits += ({ fun });
    }
}

void remove_init(mixed fun) {
    if(valid_function(fun) || stringp(fun)) {
        inits -= ({ fun });
    }
}

void clear_inits() {
    inits = ({});
}

mapping query_inits() {
    return copy(inits);
}
