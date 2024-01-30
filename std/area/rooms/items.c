private nosave mapping items = ([]);

mapping set_items(mapping itemData);
mapping remove_item(string id);
mapping add_item(mixed id, string desc);
mapping query_items() ;
string query_item(string id) ;

mapping set_items(mapping itemData) {
    mixed id;
    string key;
    mapping item = ([]);

    foreach(id, key in itemData) {
        if(pointerp(id)) {
            int i;
            for(i = 0; i < sizeof(id); i++)
                item += ([id[i] : key]);
        }
        else item += ([id : key]);
    }

    items = item;
    return query_items() ;
}

mapping query_items() {
    return copy(items) ;
}

string query_item(string id) {
    if(items[id]) {
        if(items[id][0..1] == "@@")
            return( call_other(this_object(), items[id][2..(strlen(items[id])-1)]));
        else return items[id];
    }
}

mapping remove_item(string id) {
    if(!items[id]) return query_items() ;
    map_delete(items, id);
    return query_items() ;
}

mapping add_item(mixed id, string desc) {
    if(arrayp(id)) {
        int i;

        for(i = 0; i < sizeof(id); i++) {
            if(items[id[i]]) map_delete(items, id);
            items += ([id[i] : desc]);
        }
    } else {
        if(items[id]) map_delete(items, id);
        items += ([id : desc]);
    }

    return query_items() ;
}
