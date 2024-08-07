/**
 * @file /std/consume/drink.c
 * @description Drink inheritable for objects that can be consumed.
 *
 * @created 2024/08/06 - Gesslar
 * @last_modified 2024/08/06 - Gesslar
 *
 * @history
 * 2024/08/06 - Gesslar - Created
 */

inherit STD_ITEM ;

inherit M_POTABLE ;

void mudlib_setup() {
    set_potable(1);
}

void set_id(mixed str) {
    ::set_id(str);
    add_id("drink");
}


int drink(object tp) {
    if(!::drink(tp))
        return 0 ;

    if(query_uses() < 1)
        remove() ;

    return 1 ;
}

int sip(object tp) {
    if(!::sip(tp, 1))
        return 0 ;

    if(query_uses() < 1)
        remove() ;

    return 1 ;
}

int is_drink() { return 1 ; }
