// /std/object/contents.c
// Handles the contents of an object
//
// Created:     2024/02/18: Gesslar
// Last Change: 2024/02/18: Gesslar
//
// 2024/02/18: Gesslar - Created

int max_capacity, max_volume ;
int capacity, volume ;

void clean_contents() {
    object *obs = deep_inventory() ;

    obs->clean_contents() ;
    destruct(obs) ;
}

void set_max_capacity(int x) {
    max_capacity = x ;
}

void set_max_volume(int x) {
    max_volume = x ;
}

int query_max_capacity() {
    return max_capacity ;
}

int query_max_volume() {
    return max_volume ;
}

int query_capacity() {
    return capacity ;
}

int query_volume() {
    return volume ;
}

int add_capacity(int x) {
    if (capacity + x > max_capacity) {
        return 0 ;
    }
    capacity += x ;
    return 1 ;
}

int add_volume(int x) {
    if (volume + x > max_volume) {
        return 0 ;
    }
    volume += x ;
    return 1 ;
}

int remove_capacity(int x) {
    if (capacity - x < 0) {
        return 0 ;
    }
    capacity -= x ;
    return 1 ;
}

int remove_volume(int x) {
    if (volume - x < 0) {
        return 0 ;
    }
    volume -= x ;
    return 1 ;
}