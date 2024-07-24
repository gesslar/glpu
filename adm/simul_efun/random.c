#include "/adm/obj/simul_efun.h"

/**
 * @simul_efun random_number
 * @description Generates a random integer between 0 and max-1, along with a new seed.
 * @param {int} seed - The seed for random number generation.
 * @param {int} max - The upper bound (exclusive) for the random number.
 * @returns {int[]} - An array where the first element is the new seed and
 *                    the second element is the random number.
 */
int *random_number(int seed, int max) {
    int new_seed, result;

    if (max <= 0) return ({ seed, 0 });

    new_seed = (seed * 1103515245 + 12345) & MAX_INT;
    result = new_seed % max;

    return ({ new_seed, result });
}

/**
 * @simul_efun random_float
 * @description Generates a random float between 0 and 1, along with a new seed.
 * @param {int} seed - The seed for random number generation.
 * @returns {mixed[]} - An array where the first element is the new seed and
 *                      the second element is the random float.
 */
mixed *random_float(int seed) {
    int new_seed;
    float result;

    new_seed = (seed * 1103515245 + 12345) & MAX_INT;
    result = (float)new_seed / MAX_INT;

    return ({ new_seed, result });
}

/**
 * @simul_efun random_range
 * @description Generates a random integer within a specified range, along with a new seed.
 * @param {int} seed - The seed for random number generation.
 * @param {int} min - The lower bound (inclusive) of the range.
 * @param {int} max - The upper bound (inclusive) of the range.
 * @returns {int[]} - An array where the first element is the new seed and
 *                    the second element is the random number in the specified range.
 */
int *random_range(int seed, int min, int max) {
    int *result;

    if (min >= max) return ({ seed, min });

    result = random_number(seed, max - min + 1);
    return ({ result[0], result[1] + min });
}

/**
 * @simul_efun random_element
 * @description Selects a random element from an array, along with a new seed.
 * @param {int} seed - The seed for random number generation.
 * @param {mixed[]} arr - The array to select from.
 * @returns {mixed[]} - An array where the first element is the new seed and
 *                      the second element is the randomly selected array element.
 */
mixed *random_element(int seed, mixed *arr) {
    int *result;

    if (!sizeof(arr)) return ({ seed, 0 });

    result = random_number(seed, sizeof(arr));
    return ({ result[0], arr[result[1]] });
}

/**
 * @simul_efun random_weighted
 * @description Selects a random element based on weighted probabilities, along with a new seed.
 * @param {int} seed - The seed for random number generation.
 * @param {mapping} weights - A mapping where keys are items and values are weights.
 * @returns {mixed[]} - An array where the first element is the new seed and
 *                      the second element is the randomly selected item based on the weights.
 */
mixed *random_weighted(int seed, mapping weights) {
    float total = 0.0;
    mixed *items = keys(weights);
    mixed *float_result;
    float roll;

    if (!sizeof(items)) return ({ seed, 0 });

    foreach (mixed item, int weight in weights) {
        total += weight;
    }

    float_result = random_float(seed);
    seed = float_result[0];
    roll = float_result[1] * total;

    foreach (mixed item, int weight in weights) {
        if (roll < weight) return ({ seed, item });
        roll -= weight;
    }

    return ({ seed, items[<1] });  // Shouldn't reach here, but just in case
}
