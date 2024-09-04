/**
 * @file /std/object/value.c
 * @description Handles the value of an object.
 *
 * @created 2024-08-01 - Gesslar
 * @last_modified 2024-08-08 - Gesslar
 *
 * @history
 * 2024-08-01 - Gesslar - Created
 * 2024-08-08 - Gesslar - Added save_var()
 * 2024-08-08 - Gesslar - Changed to singular value without currency type.
 */

#include <persist.h>

protected nosave int _amount = null;
/**
 * @object_function set_value
 * @description Set the value of the object. The value is stored as an integer
 *              representing the value in the lowest denomination of currency.
 * @param {int} value - The value to set.
 */

void set_value(int value) {
    save_var("_amount");

    _amount = value;
}

/**
 * @object_function query_value
 * @description Query the value of the object. The value is returned as an
 *              integer representing the value in the lowest denomination of
 *              currency.
 * @returns {int} - The value of the object.
 */
int query_value() {
    return _amount;
}

/**
 * @object_function adjust_value
 * @description Add to the value of the object. The value is added as an
 *              integer representing the value in the lowest denomination of
 *              currency.
 * @param {int} value - The value to add.
 */
void adjust_value(int value) {
    _amount += value;
}
