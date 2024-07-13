#include "/adm/obj/simul_efun.h"

/**
 * @simul_efun percent_of
 * @description Calculates what percentage `a` is of `b`.
 * @param {float} a - The part value.
 * @param {float} b - The whole value.
 * @returns {float} - The percentage of `a` in `b`.
 */
float percent_of(float a, float b) {
    return (to_float(a) / to_float(b)) * 100.0;
}

/**
 * @simul_efun percent
 * @description Calculates the percentage of `a` out of `b`.
 * @param {float} a - The part value.
 * @param {float} b - The whole value.
 * @returns {float} - The percentage of `a` out of `b`.
 */
float percent(float a, float b) {
    return to_float(a) * 100.0 / to_float(b);
}

/**
 * @simul_efun range
 * @description Ensures a value is within a specified range.
 * @param {float} min - The minimum value.
 * @param {float} max - The maximum value.
 * @param {float} val - The value to check.
 * @returns {float} - The value, constrained within the range of `min` to `max`.
 */
float range(float min, float max, float val) {
    if(val < min) return min;
    if(val > max) return max;
    return val;
}

/**
 * @simul_efun remainder
 * @description Calculates the remainder of `a` divided by `b`.
 * @param {mixed} a - The dividend.
 * @param {mixed} b - The divisor.
 * @returns {float} - The remainder of `a` divided by `b`.
 */
varargs float remainder(mixed a, mixed b) {
    int result ;

    if(intp(a)) a = to_float(a);
    if(intp(b)) b = to_float(b);

    result = a / b ;

    return result - floor(result) ;
}

/**
 * @simul_efun sum
 * @description Calculates the sum of all elements in an array.
 * @param {mixed[]} arr - The array of numbers to sum.
 * @returns {int} - The sum of all elements in the array.
 */
int sum(mixed *arr) {
    int i, total = 0;

    for(i = 0; i < sizeof(arr); i++) {
        total += arr[i];
    }

    return total;
}
