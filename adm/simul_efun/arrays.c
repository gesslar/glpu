#include "/adm/obj/simul_efun.h"

mixed *distinct_array(mixed *arr) {
    mapping m ;

    m = allocate_mapping(arr, 0) ;

    return keys(m) ;
}
