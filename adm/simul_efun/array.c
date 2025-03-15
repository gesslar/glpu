#include <simul_efun.h>

/**
 * Returns a new array containing the distinct elements of the input
 * array.
 *
 * @param {mixed[]} arr - An array of mixed types.
 * @return {mixed[]} - A new array with distinct elements from the input array.
 */
varargs mixed *distinct_array(mixed *arr, int same_order) {
  assert_arg(pointerp(arr), 1, "Array is required.");

  if(!!same_order) {
    return reduce(arr, function(mixed *acc, mixed element) {
      return member_array(element, acc) == -1
        ? acc + ({ element })
        : acc;
    }, ({}));
  } else {
    mapping m;

    m = allocate_mapping(arr, 0);

    return keys(m);
  }
}

/**
 * Returns a new array containing the elements of the input array
 * from index 0 to start-1, and from end+1 to the end of the input
 * array. If start is greater than end, the new array will contain
 * all the elements of the input array.
 *
 * @param {mixed[]} arr - The input array.
 * @param {int} start - The starting index of elements to be removed.
 * @param {int} end - The ending index of elements to be removed. Defaults to
 *                      start if not specified.
 * @return {mixed[]} - A new array with specified elements removed.
 */
varargs mixed *remove_array_element(mixed *arr, int start, int end) {
  if(!end) end = start;
  if(start > end) return arr;
  return arr[0..start-1] + arr[end+1..];
}

/**
 * Modifies the content of an array by removing existing elements
 * and/or adding new elements. Returns a new array with the
 * modifications.
 *
 * @param {mixed[]} arr - The array from which elements will be removed and to
 *                        which new elements may be added.
 * @param {int} start - The zero-based index at which to start changing the
 *                      array. If negative, it will begin that many elements
 *                      from the end.
 * @param {int} delete_count - The number of elements to remove from the array,
 *                             starting from the index specified by start. If
 *                             delete_count is 0, no elements are removed.
 * @param {mixed[]} items_to_add - An array of elements to add to the array at
 *                                   the start index. Can be omitted or passed as
 *                                   null if no elements are to be added.
 * @return {mixed[]} - A new array reflecting the desired modifications.
 */
varargs mixed *splice(mixed *arr, int start, int delete_count, mixed *items_to_add) {
  mixed *before, *after;
  if(!pointerp(items_to_add))
    items_to_add = ({});

  before = arr[0..start - 1];
  after = arr[start + delete_count..];

  return before + items_to_add + after;
}

/**
 * Returns a new array with the elements of the input array in
 * reverse order.
 *
 * @param {mixed[]} arr - The input array.
 * @return {mixed[]} - A new array with elements in reverse order.
 */
mixed *reverse_array(mixed *arr) {
  int i, j, sz;
  mixed *ret;

  for(i = 0, j = sizeof(arr) - 1, sz = sizeof(arr), ret = allocate(sz); i < sz; i++, j--)
    ret[i] = arr[j];

  return ret;
}

/**
 * Checks if all elements in the input array are of the specified
 * type. If the array is of size 0, it is considered uniform.
 *
 * @param {mixed*} arr - The array to check.
 * @param {string} type - The type to check for.
 * @return {int} - Returns 1 if all elements are of the specified type, 0
 *                 otherwise.
 */
int uniform_array(mixed *arr, string type) {
  int sz = sizeof(arr);

  if(!sz)
    return 1;

  return sizeof(filter(arr, (: typeof($1) == $2 :), type)) == sz;
}

/**
 * Returns an array filled with the specified value. If no array
 * is provided, an empty array is created. If no value is
 * provided, 0 is used as the value to fill the array with. If no
 * start index is provided, the array is filled from the end.
 *
 * @param {mixed*} arr - The array to fill.
 * @param {mixed} value - The value to fill the array with.
 * @param {int} start_index - The index at which to start filling the array. (optional)
 * @return {mixed} - The filled array.
 */
varargs mixed array_fill(mixed *arr, mixed value, int size, int start_index) {
  mixed *work;
  int len;

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
 * Returns a new array of the specified size, filled with the
 * specified value. If the array is larger than the specified size,
 * the array is truncated to the specified size.
 *
 * @param {mixed*} arr - The array to pad.
 * @param {int} size - The size of the array to create.
 * @param {mixed} value - The value to fill the array with.
 * @param {int} beginning - Whether to fill the array from the beginning. (optional)
 * @return {mixed} - The padded array.
 */
varargs mixed array_pad(mixed *arr, int size, mixed value, int beginning) {
  mixed *work;
  int i;
  int len;

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

/**
 * Removes and returns the last element of the array.
 *
 * @param {mixed[]} arr - The array from which to pop an element.
 * @return {mixed} - The last element of the array.
 */
mixed pop(mixed ref *arr) {
  mixed ret;

  ret = arr[<1];
  arr = arr[0..<2];

  return ret;
}

/**
 * Adds a new element to the end of the array and returns the new
 * size of the array.
 *
 * @param {mixed[]} arr - The array to which to push an element.
 * @param {mixed} value - The element to push onto the array.
 * @return {int} - The new size of the array.
 */
int push(mixed ref *arr, mixed value) {
  arr += ({ value });
  return sizeof(arr);
}

/**
 * Removes and returns the first element of the array.
 *
 * @param {mixed[]} arr - The array from which to shift an element.
 * @return {mixed} - The first element of the array.
 */
mixed shift(mixed ref *arr) {
  mixed ret;

  ret = arr[0];
  arr = arr[1..];

  return ret;
}

/**
 * Adds a new element to the beginning of the array and returns
 * the new size of the array.
 *
 * @param {mixed[]} arr - The array to which to unshift an element.
 * @param {mixed} value - The element to unshift onto the array.
 * @return {int} - The new size of the array.
 */
int unshift(mixed ref *arr, mixed value) {
  arr = ({ value }) + arr;
  return sizeof(arr);
}

/**
 * Returns a new array containing the elements of the input array
 * from the start index to the end index. If the end index is
 * negative, it will start from the end of the array.
 *
 * @param {mixed[]} arr - The array to slice.
 * @param {int} start - The starting index of the slice.
 * @param {int} end - The ending index of the slice.
 * @return {mixed[]} - A new array with the specified elements.
 */
varargs mixed *array_slice(mixed *arr, int start, int end) {
  if(nullp(arr))
    return ({});

  if(end < 0)
    end = sizeof(arr) + end;

  return arr[start..end];
}

/**
 * Merges arrays into a single array.
 *
 * @param {mixed[]} arrs - Comma separated arguments of arrays
 * @return {mixed[]} - A new array with elements from both input arrays.
 */
mixed *array_merge(mixed arrs...) {
  if(!uniform_array(arrs, T_ARRAY))
    throw("Syntax error: array_merge must be called with only array arguments.");

  if(sizeof(arrs) == 1)
    return arrs[0];

  return reduce(arrs, (: $1 + $2 :), ({}));
}

/**
 * Reduces an array to a single value by applying a function to each element.
 * The function accumulates a result as it processes each element.
 *
 * @param {mixed*} arr Array to reduce
 * @param {function} fun Function taking (accumulator, currentValue, index, array)
 * @param {mixed} [init=arr[0]] Initial value for accumulator
 * @return {mixed} Final accumulated value
 *
 * @example
 * ```c
 * int *nums = ({1, 2, 3, 4});
 * // Sum all numbers
 * int sum = reduce(nums, (: $1 + $2 :), 0);
 * // Multiply all numbers
 * int product = reduce(nums, (: $1 * $2 :), 1);
 * ```
 */
varargs mixed reduce(mixed *arr, function fun, mixed init, mixed arg...) {
  mixed result;
  int start, i;

  if(!pointerp(arr)) error("Bad argument 1 to reduce");
  if(!valid_function(fun)) error("Bad argument 2 to reduce");

  // If no initial value and array is empty, error
  if(!sizeof(arr) && nullp(init))
    error("Reduce of empty array with no initial value");

  // If no initial value, use first element
  if(nullp(init)) {
    result = arr[0];
    start = 1;
  } else {
    result = init;
    start = 0;
  }

  arg = pointerp(arg) && sizeof(arg) ? arg : ({});

  for(i = start; i < sizeof(arr); i++)
    result = (*fun)(result, arr[i], i, arr, arg...);

  return result;
}

int every(mixed *arr, function fun) {
  assert_arg(pointerp(arr), 1, "Array is required");
  assert_arg(valid_function(fun), 2, "Function is required");

  foreach(mixed elem in arr)
    if(!(*fun)(elem))
      return 0;

  return 1;
}

int includes(mixed *arr, mixed elem) {
  return of(elem, arr, (: $1 == $2 :));
}

private int same_array_exact(mixed *one, mixed *two) {
  int sz;

  if(sizeof(one) != sizeof(two)) return 0;

  sz = sizeof(one);
  while(sz--)
    if(one[sz] != two[sz])
      return 0;

  return 1;
}

varargs int same_array(mixed *one, mixed *two, int exact) {
  if(!!exact)
    return same_array_exact(one, two);

  return every(one, (: includes($(two), $1) :)) &&
         every(two, (: includes($(one), $1) :));
}

int some(mixed *arr, function fun) {
  assert_arg(pointerp(arr), 1, "Array is required");
  assert_arg(valid_function(fun), 2, "Function is required");

  foreach(mixed elem in arr)
    if((*fun)(elem))
      return 1;

  return 0;
}

mixed *eject(mixed ref *arr, int index) {
  mixed ret;

  ret = arr[index];
  if(index == 0)
    return shift(ref arr);
  else if(index == sizeof(arr)-1)
    return pop(ref arr);

  arr = arr[0..index-1] + arr[index+1..];

  return ret;
}

mixed insert(mixed ref *arr, mixed value, int index) {
  if(index == 0)
    return unshift(ref arr, value);
  else if(index == sizeof(arr)-1)
    return push(ref arr, value);

  arr = arr[0..index-1] + ({value}) + arr[index..];

  return sizeof(arr);
}

mixed *flatten(mixed *arr) {
  int i = 0;

  assert_arg(pointerp(arr), 1, "Array is required");

  arr = copy(arr);

  while(i < sizeof(arr))
    if(pointerp(arr[i]))
      arr[i..i] = arr[i];
    else
      i++;

  return arr;
}

varargs int find_index(mixed *arr, function fun, mixed extra...) {
  int i, sz;

  assert_arg(pointerp(arr), 1, "Array is required");
  assert_arg(valid_function(fun), 2, "Function is required");

  for(i = 0, sz = sizeof(arr); i < sz; i++)
    if(extra ? (*fun)(arr[i], extra...) : (*fun)(arr[i]) == 1)
      return i;

  return -1;
}

varargs mixed find(mixed *arr, function fun, mixed extra...) {
  int index;

  assert_arg(pointerp(arr), 1, "Array is required");
  assert_arg(valid_function(fun), 2, "Function is required");

  index = find_index(arr, fun, extra...);

  return index != -1 ? arr[index] : null;
}
