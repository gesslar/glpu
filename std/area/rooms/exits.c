private nosave mapping exits = ([]) ;
private nosave mapping _pre_exit_funcs = ([]) ;
private nosave mapping _post_exit_funcs = ([]) ;

mapping set_exits(mapping exit);
mapping remove_exit(string id);
mapping add_exit(string id, string path);
string query_exit(string id) ;
mixed query_exit_dest(string id) ;
int valid_exit(string exit) ;
mapping query_exits() ;

mapping set_exits(mapping exit) {
    exits = exit ;
    return query_exits() ;
}

mapping query_exits() {
    return copy(exits) ;
}

string query_exit(string id) {
    return exits[id] ;
}

object query_exit_dest(string id) {
    mixed dest = exits[id] ;

    if(!dest)
        return 0 ;

    if(valid_function(dest))
        dest = (*dest)() ;

    if(stringp(dest))
        dest = load_object(dest) ;

    if(!objectp(dest))
        return 0 ;

    return dest ;
}
int valid_exit(string exit) {
    return !nullp(exits[exit]) ;
}

mapping remove_exit(string id) {
    if(nullp(exits[id])) return query_exits() ;
    map_delete(exits, id) ;
    return query_exits() ;
}

mapping add_exit(string id, string path) {
    exits[id] = path ;
    return query_exits() ;
}

int has_pre_exit_funct(string dir) {
    return !nullp(_pre_exit_funcs[dir]) ;
}

int has_post_exit_funct(string dir) {
    return !nullp(_post_exit_funcs[dir]) ;
}

void add_pre_exit_func(string dir, mixed func) {
    map_delete(_pre_exit_funcs, dir) ;
    if(!valid_function(dir) && !stringp(func))
        return ;

    _pre_exit_funcs[dir] = func ;
}

void add_post_exit_func(string dir, mixed func) {
    map_delete(_post_exit_funcs, dir) ;
    if(!valid_function(dir) && !stringp(func))
        return ;

    _post_exit_funcs[dir] = func ;
}

void remove_pre_exit_func(string dir) {
    map_delete(_pre_exit_funcs, dir) ;
}

void remove_post_exit_func(string dir) {
    map_delete(_post_exit_funcs, dir) ;
}

mixed query_pre_exit_func(string dir) {
    return _pre_exit_funcs[dir] ;
}

mixed query_post_exit_func(string dir) {
    return _post_exit_funcs[dir] ;
}

void evaluate_pre_exit_func(string dir, object who) {
    mixed func = _pre_exit_funcs[dir] ;
    if(stringp(func)) {
        return catch(call_other(this_object(), func, who)) ;
    } else if(valid_function(func)) {
        return catch((*func)(who)) ;
    }
}

void evaluate_post_exit_func(string dir, object who) {
    mixed func = _post_exit_funcs[dir] ;
    if(stringp(func)) {
        return catch(call_other(this_object(), func, who)) ;
    } else if(valid_function(func)) {
        return catch((*func)(who)) ;
    }
}
