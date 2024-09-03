#include <simul_efun.h>

/**
 * @simul_efun distinct_array
 * @description Returns a new array containing the distinct elements of the input
 *              array.
 * @param {mixed[]} arr - An array of mixed types.
 * @returns {mixed[]} - A new array with distinct elements from the input array.
 */
mixed *distinct_array(mixed *arr) {
    mapping m;

    m = allocate_mapping(arr, 0);

    return keys(m);
}

/**
 * @simul_efun remove_array_element
 * @description Returns a new array containing the elements of the input array
 *              from index 0 to start-1, and from end+1 to the end of the input
 *              array. If start is greater than end, the new array will contain
 *              all the elements of the input array.
 * @param {mixed[]} arr - The input array.
 * @param {int} start - The starting index of elements to be removed.
 * @param {int} end - The ending index of elements to be removed. Defaults to
 *                      start if not specified.
 * @returns {mixed[]} - A new array with specified elements removed.
 */
varargs mixed *remove_array_element(mixed *arr, int start, int end) {
    if(!end) end = start;
    if(start > end) return arr;
    return arr[0..start-1] + arr[end+1..];
}

/**
 * @simul_efun splice
 * @description Modifies the content of an array by removing existing elements
 *              and/or adding new elements. Returns a new array with the
 *              modifications.
 * @param {mixed[]} arr - The array from which elements will be removed and to
 *                        which new elements may be added.
 * @param {int} start - The zero-based index at which to start changing the
 *                      array. If negative, it will begin that many elements
 *                      from the end.
 * @param {int} delete_count - The number of elements to remove from the array,
 *                             starting from the index specified by start. If
 *                             delete_count is 0, no elements are removed.
 * @param {mixed[]} [items_to_add] - An array of elements to add to the array at
 *                                   the start index. Can be omitted or passed as
 *                                   null if no elements are to be added.
 * @returns {mixed[]} - A new array reflecting the desired modifications.
 */
varargs mixed *splice(mixed *arr, int start, int delete_count, mixed *items_to_add) {
    mixed *before, *after;
    if(!pointerp(items_to_add)) {
        items_to_add = ({});
    }

    before = arr[0..start - 1];
    after = arr[start + delete_count..];

    return before + items_to_add + after;
}

/**
 * @simul_efun reverse_array
 * @description Returns a new array with the elements of the input array in
 *              reverse order.
 * @param {mixed[]} arr - The input array.
 * @returns {mixed[]} - A new array with elements in reverse order.
 */
mixed *reverse_array(mixed *arr) {
    int i, j, sz;
    mixed *ret;

    for(i = 0, j = sizeof(arr) - 1, sz = sizeof(arr), ret = allocate(sz); i < sz; i++, j--) {
        ret[i] = arr[j];
    }

    return ret;
}

/**
 * @simul_efun uniform_array
 * @description Checks if all elements in the input array are of the specified
 *              type. If the array is of size 0, it is considered uniform.
 * @param {string} type - The type to check for.
 * @param {mixed*} arr - The array to check.
 * @returns {int} - Returns 1 if all elements are of the specified type, 0
 *                  otherwise.
 */
int uniform_array(string type, mixed *arr) {
    int sz = sizeof(arr);

    if(!sz)
        return 1;

    return sizeof(filter(arr, (: typeof($1) == $2 :), type)) == sz;
}

/**
 * @simul_efun array_fill
 * @description Returns an array filled with the specified value. If no array
 *              is provided, an empty array is created. If no value is
 *              provided, 0 is used as the value to fill the array with. If no
 *              start index is provided, the array is filled from the end.
 * @param {mixed*} arr - The array to fill.
 * @param {mixed} value - The value to fill the array with.
 * @param {int} start_index - The index at which to start filling the array. (optional)
 * @return {mixed}
 */
varargs mixed array_fill(mixed *arr, mixed value, int size, int start_index) {
    mixed *work, *ret;
    int i;
    int len ;

    if(!pointerp(arr))
        arr = ({});

    if(nullp(value))
        value = 0;

    if(nullp(size))
        error("array_fill: size is required");

    len = sizeof(arr);

    if(nullp(start_index))
        start_index = len;

    work = allocate(size);

    while(size--)
        work[size] = value;

    return arr[0..start_index-1] + work + arr[start_index..];
}

/**
 * @simul_efun array_pad
 * @description Returns a new array of the specified size, filled with the
 *              specified value. If the array is larger than the specified size,
 *              the array is truncated to the specified size.
 * @param {mixed*} arr - The array to pad.
 * @param {int} size - The size of the array to create.
 * @param {mixed} value - The value to fill the array with.
 * @param {int} beginning - Whether to fill the array from the beginning. (optional)
 * @return {mixed}
 */
varargs mixed array_pad(mixed *arr, int size, mixed value, int beginning) {
    mixed *work, *ret;
    int i;
    int len ;

    if(!pointerp(arr))
        arr = ({});

    if(nullp(value))
        value = 0;

    len = sizeof(arr);

    if(size <= len)
        return arr[0..size-1];

    work = allocate(size - len);

    while(size--)
        work[size] = value;

    if(beginning)
        return work + arr;
    else
        return arr + work;
}
