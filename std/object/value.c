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
 */

#include <persist.h>

private nosave int _amount = null;
private nosave string _currency = null ;

/**
 * @description Set the value of the object.
 * @param {int} value - The value to set.
 * @param {string} currency - The currency to set.
 */

void set_value(int value, string currency) {
    save_var("_amount", "_currency");

    _amount = value;
    _currency = currency;
}

/**
 * @description Query the value of the object.
 * @returns {mixed[]} - The value of the object. First element is the amount,
 *                      second element is the currency.
 */
mixed *query_value() {
    return ({ _amount, _currency });
}

/**
 * @description Add to the value of the object.
 * @param {int} value - The value to add.
 */

void adjust_value(int value) {
    _amount += value;
}
