/**
 * @file /std/object/container.c
 * @description Container inheritable for objects that can hold other objects.
 *
 * @created 2024/07/23 - Gesslar
 * @last_modified 2024/07/23 - Gesslar
 *
 * @history
 * 2024/07/23 - Gesslar - Created
 */

inherit __DIR__ "inventory" ;
inherit __DIR__ "contents" ;

private nosave int _ignore_capacity = 0 ;
private nosave int _ignore_mass = 0 ;

int can_receive(object ob) {
    return 1;
}
void receive_effect(object ob) {}

int can_release(object ob) {
    return 1;
}
void release_effect(object ob) {}

void set_ignore_capacity(int i) {
    rehash_capacity() ;
    _ignore_capacity = !!i ;
}

int ignore_capacity() {
    return _ignore_capacity ;
}

void set_ignore_mass(int i) {
    rehash_capacity() ;
    _ignore_mass = !!i ;
}

int ignore_mass() {
    return _ignore_mass ;
}

int is_container() { return 1 ; }
