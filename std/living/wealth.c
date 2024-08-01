// /std/user/wealth.c
// Wealth management for livings
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

#include <wealth.h>

// Functions from other objects
int query_capacity() ;
void rehash_capacity() ;

private nomask mapping wealth = ([]) ;

int query_total_coins() {
    int total ;

    total = 0 ;

    foreach(int c in values(wealth)) {
        total += c ;
    }

    return total ;
}

int query_total_wealth() {
    int total = 0 ;
    mixed *config = mud_config("CURRENCY") ;

    foreach(mixed *c in config) {
        total += wealth[c[0]] * c[1] ;
    }
    return total ;
}

mapping query_all_wealth() {
    return copy(wealth) ;
}

int query_wealth(string currency) {
    return wealth[currency] ;
}

int add_wealth(string currency, int amount) {
    if(nullp(wealth[currency]))
        return null ;

    if(amount < 0)
        if(wealth[currency] - amount < 0)
            return null ;

    wealth[currency] += amount ;

    rehash_capacity() ;

    return wealth[currency] ;
}

mapping set_wealth(mapping w) {
    mixed *config = mud_config("CURRENCY") ;

    foreach(mixed *c in config) {
        if(nullp(w[c[0]])) w[c[0]] = 0 ;
    }

    rehash_capacity() ;

    return wealth = w ;
}

// This is only useful for livings that can carry wealth
// and checks if the living has the capacity for it.
int can_carry_wealth(int amount) {
    int cap = query_capacity() ;

    return amount <= cap ;
}
