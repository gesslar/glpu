/**
 * @file /std/area/corpse/corpse.c
 * @description Corpses oooOooOooOooO
 *
 * @created 2024/07/27 - Gesslar
 * @last_modified 2024/07/27 - Gesslar
 *
 * @history
 * 2024/07/27 - Gesslar - Created
 */

inherit STD_ITEM ;
inherit STD_CONTAINER ;

private string killer_name ;
private string dead_name ;

void setup_corpse(object dead, object killer) {
    killer_name = killer ? killer->query_name() : "unknown" ;
    dead_name = dead->query_name() ;

    set_name(dead_name+"'s' corpse") ;
    set_short("The corpse of "+dead_name) ;
    set_long("This is the corpse of "+dead_name+".") ;
    set_mass(1) ;
    set_bulk(1) ;
    set_max_capacity(1000) ;
    set_max_volume(1000) ;
}
