#include "/adm/obj/simul_efun.h"

//:FUNCTION distinct_array
//Synopsis: mixed *distinct_array(mixed *arr)
//
//Returns a new array containing the distinct elements of the input array.
//The order of the elements in the new array is not guaranteed to be the same
//as the order of the elements in the input array.
mixed *distinct_array(mixed *arr) {
    mapping m ;

    m = allocate_mapping(arr, 0) ;

    return keys(m) ;
}
