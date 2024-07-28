// /std/equip/clothing.c
// Basic clothing object
//
// Created:     2024/03/02: Gesslar
// Last Change: 2024/03/02: Gesslar
//
// 2024/03/02: Gesslar - Created

inherit STD_EQUIP ;

private mapping covers = ([ ]) ;

private void set_covers(mapping sl) ;
private void remove_cover(string slot) ;
private void remove_covers(mapping sl) ;
public mapping query_covers() ;
private void add_cover(string slot) ;
int is_clothing() ;

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

void unsetup() {
    unequip(environment()) ;
}

int is_clothing() { return 1; }
