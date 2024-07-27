// /std/equip/clothing.c
// Basic clothing object
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_EQUIP ;

private mapping slots = ([ ]) ;
private mapping covers = ([ ]) ;

public void set_slots(mapping sl) ;
private void add_slot(string slot, string type) ;
private void remove_slot(string slot) ;
public mapping query_slots() ;
public string query_slot_type(string slot) ;
private void set_covers(mapping sl) ;
private void remove_cover(string slot) ;
private void remove_covers(mapping sl) ;
public mapping query_covers() ;
private void add_cover(string slot) ;
int is_clothing() ;

mixed equip(object tp) {
    return call_if(this_object(), "equip_check", tp) || 1 ;
}

public void set_slots(mapping sl) {
    slots = sl ;
}

private void add_slot(string slot, string type) {
    if(!slots) slots = ([ ]) ;
    slots[slot] = type ;
}

private void remove_slot(string slot) {
    if(!slots) return ;
    map_delete(slots, slot) ;
}

public mapping query_slots() {
    return slots ;
}

public string query_slot_type(string slot) {
    if(!slots) return 0 ;
    return slots[slot] ;
}

private void set_covers(mapping sl) {
    foreach(string slot, string type in sl) {
        add_cover(slot) ;
    }
}

private void remove_cover(string slot) {
    if(!covers) return ;
    map_delete(covers, slot) ;
}

private void remove_covers(mapping sl) {
    foreach(string slot, string type in sl) {
        remove_cover(slot) ;
    }
}

public mapping query_covers() {
    return covers ;
}

private void add_cover(string slot) {
    switch(slot) {
        case "head":
            covers["head"] = 1.0 ;
            break ;
        case "torso":
            covers["torso"] = 1.0 ;
            break ;
        case "legs":
            covers["legs"] = 1.0 ;
            break ;
        case "arms":
            covers["arms"] = 1.0 ;
            break ;
        case "hands":
            covers["hands"] = 1.0 ;
            break ;
        case "feet":
            covers["feet"] = 1.0 ;
            break ;
        default:
            break ;
    }
}

int is_clothing() { return 1; }
