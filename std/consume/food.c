/**
 * @file /std/consume/food.c
 * @description Food inheritable for objects that can be consumed.
 *
 * @created 2024/08/06 - Gesslar
 * @last_modified 2024/08/06 - Gesslar
 *
 * @history
 * 2024/08/06 - Gesslar - Created
 */

inherit STD_ITEM ;
inherit M_EDIBLE ;

void mudlib_setup() {
    set_edible(1);
}

void set_id(mixed str) {
    ::set_id(str);
    add_id("food");
}

int consume(object tp) {
    if(!::consume(tp))
        return 0 ;

    remove() ;

    return 1 ;
}

int nibble(object tp) {
    if(!::nibble(tp, 1))
        return 0 ;

    if(query_uses() < 1)
        remove() ;

    return 1 ;
}

int is_food() { return 1 ; }
