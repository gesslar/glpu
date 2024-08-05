// /std/object/weight.c
// Handles bulk and mass of objects
//
// Created:     2024/02/18: Gesslar
// Last Change: 2024/02/18: Gesslar
//
// 2024/02/18: Gesslar - Created

int mass, bulk;

int query_mass() {
    return mass;
}

int query_bulk() {
    return bulk;
}

int set_mass(int x) {
    if(x < 1)
        return 1 ;

    if(environment()) {
        int _capacity = environment()->query_capacity();
        if(_capacity < x) {
            return 0;
        }
        environment()->adjust_capacity(-x);
    }
    mass = x;
    return 1;
}

int set_bulk(int x) {
    if(x < 1)
        return 1;

    if(environment()) {
        int _volume = environment()->query_volume();
        if(_volume < x) {
            return 0;
        }
        environment()->adjust_volume(-x) ;
    }
    bulk = x;

    return 1;
}

int adjust_mass(int x) {
    if(x == 0)
        return 1;

    if(environment()) {
        if(x < 0) {
            int _max_capacity = environment()->query_max_capacity();
            if(_max_capacity < mass + x) {
                return 0;
            }
        } else {
            int _capacity = environment()->query_capacity();
            if(_capacity < mass + x) {
                return 0;
            }
        }
        environment()->adjust_capacity(-(mass+x));
    }
    mass += x;

    return 1;
}

int adjust_bulk(int x) {
    if(x == 0)
        return 1;

    if(environment()) {
        if(x < 0) {
            int _max_volume = environment()->query_max_volume();
            if(_max_volume < bulk + x) {
                return 0;
            }
        } else {
            int _volume = environment()->query_volume();
            if(_volume < bulk + x) {
                return 0;
            }
        }
        environment()->adjust_volume(-(bulk+x));
    }
    bulk += x;
    return 1;
}
