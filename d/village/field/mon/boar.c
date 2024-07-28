/**
 * @file /d/village/field/mon/boar.c
 * @description A boar monster
 *
 * @created 2024/07/24 - Gesslar
 * @last_modified 2024/07/24 - Gesslar
 *
 * @history
 * 2024/07/24 - Gesslar - Created
 */

inherit STD_NPC ;

void setup() {
    set_name("wild boar") ;
    set_short("wild boar") ;
    set_long("A large wild boar with sharp tusks and a bad attitude.") ;
    set_id(({ "boar", "wild boar" })) ;

    set_level(1) ;
    set_race("human") ;
}

private nomask int m() { return 1 ; }
