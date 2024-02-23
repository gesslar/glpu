// /obj/mudlib/coin.c
// Short description of this file and its purpose.
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

inherit STD_OBJECT ;

string coin_type ;
int coin_num ;
int value ;

void set_up(string type, int num) ;

void mudlib_setup() {
    set_up(null, null) ;
}

void set_up(string type, int num) {
    mixed *config = mud_config("CURRENCY") ;
    int conversion ;

    if(nullp(type) || nullp(num) || num < 1) {
        return set_up(config[0][0], 1) ;
    }

    foreach(mixed *c in config) {
        if(c[0] == type) {
            conversion = c[1] ;
            break ;
        }
    }

    if(nullp(conversion)) {
        remove() ;
        error("Invalid coin type: " + type) ;
    }

    coin_type = type ;
    coin_num = num ;
    value = num * conversion ;

    if(num == 1) {
        set_id( ({ "coin", type }) ) ;
        set_short(type + " coin") ;
        set_long("A " + type + " coin.") ;
    } else {
        set_id( ({ "coins", type + " coins" }) ) ;
        set_short(num + " " + type + " coins") ;
        set_long("A pile of " + num + " " + type + " coins.") ;
    }

    set_mass(num) ;
}

void event_moved(object prev, object previous_environment) {
    object env = environment() ;

    if(living(env)) {
        int cap = env->query_capacity() ;
        if(cap < coin_num) {
            tell(env, "You can't carry any more " + coin_type + " coins.") ;
            if(previous_environment) {
                move(previous_environment) ;
            } else {
                remove() ;
            }
        } else {
            add_mass(-coin_num) ;
            env->add_wealth(coin_type, coin_num) ;
            printf("You pick up %d %s coins.\n", coin_num, coin_type) ;
            remove() ;
        }
    }
}
