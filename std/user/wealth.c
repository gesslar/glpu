// /std/user/wealth.c
// Wealth management for livings
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

// Functions from other objects
int query_capacity() ;
void rehash_capacity() ;

mapping wealth ;

void init_wealth() {
    if(!wealth) {
        mixed *config = mud_config("CURRENCY") ;
        if(!wealth) wealth = ([ ]) ;
        foreach(mixed *c in config) {
            if(nullp(wealth[c[0]]))
                wealth[c[0]] = 0 ;
        }
    }

}

int query_total_coins() {
    int total ;

    init_wealth() ;
    total = 0 ;

    foreach(int c in values(wealth)) {
        total += c ;
    }

    return total ;
}

int query_total_wealth() {
    int total = 0 ;
    mixed *config = mud_config("CURRENCY") ;

    init_wealth() ;
    foreach(mixed *c in config) {
        total += wealth[c[0]] * c[1] ;
    }
    return total ;
}

mapping query_all_wealth() {
    init_wealth() ;
    return copy(wealth) ;
}

int query_wealth(string currency) {
    init_wealth() ;
    return wealth[currency] ;
}

int add_wealth(string currency, int amount) {
    init_wealth() ;
    if(nullp(wealth[currency])) return 0 ;

    if(amount < 0) {
        if(wealth[currency] + amount < 0)
            return 0 ;
    }

    wealth[currency] += amount ;

    rehash_capacity() ;

    return wealth[currency] ;
}

void set_wealth(mapping w) {
    mixed *config = mud_config("CURRENCY") ;

    init_wealth() ;

    foreach(mixed *c in config) {
        if(nullp(w[c[0]])) w[c[0]] = 0 ;
    }

    rehash_capacity() ;
}

// This is only useful for livings that can carry wealth
// and checks if the living has the capacity for it.
int can_carry_wealth(int amount) {
    int cap = query_capacity() ;

    return amount <= cap ;
}
