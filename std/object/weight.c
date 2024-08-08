// /std/object/weight.c
// Handles mass of objects
//
// Created:     2024/02/18: Gesslar
// Last Change: 2024/02/18: Gesslar
//
// 2024/02/18: Gesslar - Created

int mass ;

int query_mass() {
    return mass;
}

int set_mass(int x) {
_debug("%O set_mass: %O\n", this_object(), x);
    if(x < 0)
        return 0 ;

    if(environment()) {
        int capacity = environment()->query_capacity();
        if(x > capacity)
            return 0;
        if(x < 0)
            return 0;

        environment()->adjust_fill(x);
    }
    mass = x;
    return 1;
}

int adjust_mass(int x) {
    int new_mass = mass + x;
_debug("%O mass: %O new_mass: %O\n", this_object(), x, new_mass);
    if(x == 0)
        return 1;

    if(environment()) {
        int ignore_cap = environment()->ignore_capacity();
        int ignore_mass = environment()->ignore_mass();
        int cap = environment()->query_capacity();

        if(!ignore_cap && !ignore_mass) {
            if(!ignore_cap) {
                if(new_mass > cap || new_mass < 0)
                    return 0;
            }
            if(!ignore_mass) {
                if(!environment()->adjust_mass(x))
                    return 0;
            }

            environment()->adjust_fill(x);
        }

    }
    mass = new_mass;

    return 1;
}
