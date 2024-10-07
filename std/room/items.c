private nosave mapping items = ([]) ;

mapping set_items(mapping item_data) ;
mapping remove_item(string id) ;
mapping add_item(mixed id, string desc) ;
mapping query_items() ;
string query_item(string id) ;

mapping set_items(mapping item_data) {
    mixed id ;
    string key ;
    mapping item = ([]) ;

    foreach(id, key in item_data) {
        if(pointerp(id)) {
            int i ;
            for(i = 0; i < sizeof(id); i++)
                item += ([id[i] : key]) ;
        }
        else item += ([id : key]) ;
    }

    items = item ;
    return query_items() ;
}

mapping query_items() {
    return copy(items) ;
}

string query_item(string id) {
    mixed item, value ;
    string result ;

    if(!nullp(item))
        return 0 ;

    foreach(item, value in items) {
        if(pointerp(item)) {
            int i ;
            for(i = 0; i < sizeof(item); i++) {
                if(item[i] == id) {
                    result = value ;
                    break ;
                }
            }
        } else {
            if(item == id) {
                result = value ;
                break ;
            }
        }
    }

    if(valid_function(result))
        result = (*result)(id) ;

    if(stringp(result) && result[0..1] == "@@")
        return call_other(this_object(), result[2..], id) ;

    if(stringp(result))
        return result ;

    return 0 ;
}

mapping remove_item(string id) {
    if(!items[id]) return query_items() ;
    map_delete(items, id) ;
    return query_items() ;
}

mapping add_item(mixed id, string desc) {
    if(pointerp(id)) {
        int i ;

        for(i = 0; i < sizeof(id); i++) {
            if(items[id[i]]) map_delete(items, id) ;
            items += ([id[i] : desc]) ;
        }
    } else {
        if(items[id]) map_delete(items, id) ;
        items += ([id : desc]) ;
    }

    return query_items() ;
}
