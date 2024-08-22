#include <daemons.h>
#include <gmcp_defines.h>

private string get_item_type(object item) ;
private string get_item_location(object who, object item) ;

void List(object who, object container) {
    mapping data, *item_data = ({}) ;
    object item, *items ;
    string location ;

    if(!environment(who))
        return ;

    items = find_targets(who, null, container) ;
    items -= ({ who }) ;

    foreach(item in items) {
        string type ;

        type = get_item_type(item) ;

        item_data += ({ ([
            "name": get_short(item),
            "id"  : item->query_ids(),
            "type": type,
            "hash": hash("md4", file_name(item)),
        ]) }) ;
    }

    location = get_item_location(who, container) ;

    data = ([
        "items" : item_data,
        "location" : location,
    ]) ;

    who->do_gmcp(GMCP_PKG_ITEM_LIST, data, 1) ;
}

void Add(object who, object *obs) {
    object item = obs[0] ;
    object dest = obs[1] ;
    string type ;
    mapping data ;

    type = get_item_type(item) ;

    data = ([
        "name"    : get_short(item),
        "id"      : item->query_ids(),
        "type"    : type,
        "location": get_item_location(who, dest),
        "hash"    : hash("md4", file_name(item)),
    ]) ;

    who->do_gmcp(GMCP_PKG_ITEM_ADD, data, 1) ;
}

void Remove(object who, object *obs) {
    object item = obs[0] ;
    object prev = obs[1] ;
    mapping data ;

    data = ([
        "location": get_item_location(who, prev),
        "hash"    : hash("md4", file_name(item)),
    ]) ;

    who->do_gmcp(GMCP_PKG_ITEM_REMOVE, data, 1) ;
}

void Update(object who, object *obs) {
    object item = obs[0] ;
    object dest = obs[1] ;
    mapping data ;

    if(!environment(who))
        return ;

    data = ([
        "name"    : get_short(item),
        "id"      : item->query_ids(),
        "type"    : get_item_type(item),
        "location": get_item_location(who, dest),
        "hash"    : hash("md4", file_name(item)),
    ]) ;

    who->do_gmcp(GMCP_PKG_ITEM_UPDATE, data, 1) ;
}

private string get_item_type(object item) {
    string type ;

    if(item->is_armour())
        type = "armour" ;
    else if(item->is_weapon())
        type = "weapon" ;
    else if(item->is_food())
        type = "food" ;
    else if(item->is_drink())
        type = "drink" ;
    else if(item->is_clothing())
        type = "clothing" ;
    else if(item->is_npc())
        type = "npc" ;
    else if(item->is_pc())
        type = "pc" ;
    else if(item->is_tool())
        type = "tool" ;
    else
        type = "misc" ;

    return type ;
}

private string get_item_location(object who, object container) {
    string location ;

    if(container == who)
        location = "inventory" ;
    else if(container == environment(who))
        location = "room" ;
    else if(environment(container) == who)
        location = "container" ;
    else
        location = "unknown" ;

    return location ;
}
