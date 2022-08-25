private nosave mapping exits = ([]) ;

void set_exits(mapping exit);
int remove_exit(string id);
int add_exit(string id, string path);
string query_exit(string id) ;
int valid_exit(string exit) ;
mapping query_exits() ;

mapping set_exits(mapping exit)
{
    exits = exit ;
    return query_exits() ;
}

mapping query_exits()
{
    return copy(exits) ;
}

string query_exit(string id)
{
    return exits[id] ;
}

int valid_exit(string exit)
{
    return !nullp(exits[exit]) ;
}

mapping remove_exit(string id)
{
    if(nullp(exits[id])) return query_exits() ;
    map_delete(exits, id) ;
    return query_exits() ;
}

mapping add_exit(string id, string path)
{
    exits[id] = path ;
    return query_exits() ;
}
