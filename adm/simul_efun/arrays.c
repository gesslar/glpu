#include "/adm/obj/simul_efun.h"

//:FUNCTION distinct_array
//Synopsis: mixed *distinct_array(mixed *arr)
//
// Arg: mixed *arr - an array of mixed types
//
//Returns a new array containing the distinct elements of the input array.
//The order of the elements in the new array is not guaranteed to be the same
//as the order of the elements in the input array.
mixed *distinct_array(mixed *arr) {
    mapping m ;

    m = allocate_mapping(arr, 0) ;

    return keys(m) ;
}

//:FUNCTION remove_array_element
// Synopsis: mixed *remove_array_element(mixed *arr, int start, int end)
//
// Returns a new array containing the elements of the input array from index
// 0 to start-1, and from end+1 to the end of the input array.  If start is
// greater than end, the new array will contain all the elements of the input
// array.
//
// If end is not specified, it defaults to start.
varargs mixed *remove_array_element(mixed *arr, int start, int end) {
    if( !end ) end = start ;
    if( start > end ) return arr ;
    return arr[0..start-1] + arr[end+1..] ;
}

//:FUNCTION splice
//Synopsis: varargs mixed *splice(mixed *arr, int start, int deleteCount, mixed *itemsToAdd)
//
// Args:
//   mixed *arr - The array from which elements will be removed and to which new elements may be added.
//   int start - The zero-based index at which to start changing the array.
//     If negative, it will begin that many elements from the end.
//   int deleteCount - The number of elements to remove from the array, starting from the index specified by start.
//     If deleteCount is 0, no elements are removed. In this case, you should specify at least one new element.
//   mixed *itemsToAdd - (Optional) An array of elements to add to the array at the start index.
//     If no elements are to be added, this parameter can be omitted or passed as null.
// Returns:
//   A new array reflecting the desired modifications: elements specified by deleteCount are removed starting from
//   the start index, and the elements in itemsToAdd are inserted at the start index. The function does not modify
//   the original array but returns a new array with the modifications.
//
// Description:
//   The splice function modifies the content of an array by removing existing elements and/or adding new elements.
//   The original array is not altered; instead, a new array that represents the state of the array after the
//   modifications have been applied is returned. This function is useful for manipulating arrays when you need to
//   dynamically adjust their contents without directly altering the original array.
varargs mixed *splice(mixed *arr, int start, int deleteCount, mixed *itemsToAdd) {
    mixed *before, *after ;
    if (!pointerp(itemsToAdd)) { // Check if itemsToAdd is not an array
        itemsToAdd = ({}); // Set to an empty array if null or not provided
    }

    before = arr[0..start - 1]; // Elements before the starting index
    after = arr[start + deleteCount..]; // Elements after the removed elements

    // Combining the parts with the new items
    return before + itemsToAdd + after;
}
