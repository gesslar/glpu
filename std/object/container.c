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

int can_receive(object ob) {
    return 1;
}

void receive_effect(object ob) {}

int can_release(object ob) {
    return 1;
}
void release_effect(object ob) {}
