/**
 * @file /std/modules/mobile/race/human.c
 * @description Human racial module
 *
 * @created 2024/07/25 - Gesslar
 * @last_modified 2024/07/25 - Gesslar
 *
 * @history
 * 2024/07/25 - Gesslar - Created
 */

inherit __DIR__ "race" ;

int set_up_body_parts(object ob, mixed args...) {
    return use_default_body_parts() ;
}
