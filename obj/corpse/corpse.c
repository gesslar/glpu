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

void decay(int it) ;

void setup_corpse(object dead, object killer) {
    killer_name = killer ? killer->query_name() : "unknown" ;
    dead_name = dead->query_name() ;
    set_id(({ "body", "dead body", "corpse" })) ;
    set_name(dead_name+"'s' body") ;
    set_short("the dead body of "+dead_name) ;
    set_long("This is the dead body of "+dead_name+".") ;
    set_mass(1) ;
    set_capacity(1000) ;

    call_out_walltime((: decay, 0 :), 1.0) ;
}

void decay(int it) {
    if(!environment())
        return ;

    switch(it) {
        case 0:
            // don't do anything
            break ;
        case 1:
            set_short("the decaying body of "+dead_name) ;
            break ;
        case 2:
            set_short("the rotting body of "+dead_name) ;
            break ;
        case 2:
            set_short("the putrid body of "+dead_name) ;
            break ;
        case 4:
            set_short("the skeletal remains of "+dead_name) ;
            break ;
        default:
            clean_contents() ;
            remove() ;
            return ;
    }

    call_out_walltime((: decay, ++it :), 10.0) ;
}

int is_corpse() { return 1 ; }
