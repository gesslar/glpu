#include <simul_efun.h>

private nosave int _system_seed = random(time()) ;

// xorshift.h
// Pseudo-random repeatable number generator
// Kryssa@SWmud, August 2022
// Taken from "Xorshift RNGs" by George Marsaglia at Florida State University

//upper limit of 32-bit int
#define XORSHIFT_32_MAX     0xFFFFFFFF

//make sure the seed is within 1 to 4294967295
private int _sanitize_seed(int seed) {
  return (abs(seed) % XORSHIFT_32_MAX) || XORSHIFT_32_MAX;
}

//pseudo-random number via seed single step.
//Minimum seed 1 enforced by override.
//Maximum seed 4294967295 enforced by override.
private int _step_xorshift32(int seed) {
  //Force seed to stay between 1 and max uint32
  int register = _sanitize_seed(seed);

  //This seems dumb, but we're on 64 bit signed-only.
  //So, we've gotta fake unsigned 32 bit.
  register = (register ^ (register << 13)) & XORSHIFT_32_MAX;
  register = (register ^ (register >> 17)) & XORSHIFT_32_MAX;
  register = (register ^ (register <<  5)) & XORSHIFT_32_MAX;

  return register;
}

//seed generator for within the range of the xorshift32
//Just grab a random within 1 to 4294967295
int seed_generator() {
  return random( XORSHIFT_32_MAX ) + 1;
}

//get a psuedo-random number using a random number seed.
//returns the seed_result class containing the next seed and the result
int *prandom(int seed, int size) {
  int *result = allocate(2) ;
  int work ;

  if(seed == -1) {
    _system_seed = _step_xorshift32(_system_seed);
    work = _system_seed ;
    result[0] = -1 ;
  } else {
    seed = _sanitize_seed(seed);
    result[0] = _step_xorshift32(seed);
    work = result[0] ;
  }

  result[1] = work % size;

  return result;
}

float *prandom_float(int seed, float size) {
  mixed *result = allocate(2) ;
  int work ;

  if(seed == -1) {
    _system_seed = _step_xorshift32(_system_seed);
    work = _system_seed ;
    result[0] = -1 ;
  } else {
    seed = _sanitize_seed(seed);
    result[0] = _step_xorshift32(seed);
    work = result[0] ;
  }

  // Update to generate a float between 0 and size
  result[1] = (work % to_int(size)) * (size / to_int(size));

  return result;
}

//shuffle an array using a random number seed.
//returns the seed_result class containing the next seed and the result
mixed *pshuffle(int seed, mixed *arr) {
  mixed *new_arr ;
  int index ;
  mixed *result = allocate(2) ;
  int work ;

  //if null or not array return 0;
  if(!arr || !arrayp(arr))
    return 0;

  if(seed == -1) {
    _system_seed = _step_xorshift32(_system_seed);
    work = _system_seed ;
    result[0] = -1 ;
  } else {
    seed = _sanitize_seed(seed);
    result[0] = _step_xorshift32(seed);
    work = result[0] ;
  }

  index = sizeof(arr);
  new_arr = copy(arr);

  while(index--) {
    int random_index = work % index;
    mixed tmp;

    tmp = new_arr[index];
    new_arr[index] = new_arr[random_index];
    new_arr[random_index] = tmp;
    work = _step_xorshift32(work);
  }

  if(result[0] == -1)
    _system_seed = work ;

  result[1] = new_arr;

  return result;
}

//gets an element from an array using a random number seed.
//returns the seed_result class containing the next seed and the result
mixed *pelement_of(int seed, mixed *arr) {
  mixed *result = allocate(2) ;
  int work ;

  //if no items or not an array return 0;
  if(!sizeof(arr) || !arrayp(arr))
    return 0;

  if(seed == -1) {
    _system_seed = _step_xorshift32(_system_seed);
    result[0] = -1 ;
    work = _system_seed ;
  } else {
    seed = _sanitize_seed(seed);
    result[0] = _step_xorshift32(seed);
    work = result[0] ;
  }

  result[1] = arr[work % sizeof(arr)];

  return result;
}

int *prandom_range(int seed, int min, int max) {
  int *result ;

  if(min >= max)
    return ({ seed, min });

  result = prandom(seed, max - min + 1);
  result[1] += min;

  return result;
}

/**
 * @simul_efun pelement_of_weighted
 * @description Selects a random element based on weighted probabilities, along with a new seed.
 * @param {int} seed - The seed for random number generation.
 * @param {mapping} weights - A mapping where keys are items and values are weights.
 * @returns {mixed[]} - An array where the first element is the new seed and
 *                      the second element is the randomly selected item based on the weights.
 */
mixed *pelement_of_weighted(int seed, mapping weights) {
  float total = 0.0;
  mixed *items = keys(weights);
  mixed *float_result;
  float roll;
  mixed *result = allocate(2) ;

  if(!sizeof(items))
    return 0 ;

  total = sum(values(weights));

  float_result = prandom_float(seed, total);
  result[0] = float_result[0];
  roll = float_result[1];

  foreach(mixed item, int weight in weights) {
    if(roll < weight) {
      result[1] = item ;
      break ;
    }
    roll -= weight;
  }

  return result;
}

/**
 * @simul_efun random_float
 * @description Generates a random float between 0 and upper_bound.
 * @param {mixed} upper_bound - The upper bound for the random float.
 * @returns {float} - The random float between 0 and upper_bound.
 */
float random_float(mixed upper_bound) {
  if(upper_bound < 0.0)
    return 0.0;

  // Convert upper_bound to float
  upper_bound = to_float(upper_bound);

  // Generate a random float between 0 and upper_bound
  return to_float(random(to_int(upper_bound * 1000000))) / 1000000.0;
}

/**
 * @simul_efun element_of_weighted
 * @description Selects an element from a weighted mapping based on their weights.
 * @param {mapping} m - The weighted mapping to select from, where keys are the
 *                      elements and values are their weights.
 * @returns {mixed} - The selected element.
 */
mixed element_of_weighted(mapping m) {
  int total ;
  int roll ;
  mixed result ;

  total = sum(values(m)) ;

  roll = random(total) ;
  foreach(mixed item, int weight in m) {
    if(roll < weight) {
      result = item ;
      break ;
    }
    roll -= weight ;
  }

  return result ;
}

/**
 * @simul_efun random_range
 * @description Generates a random integer within a specified range.
 * @param {int} min - The lower bound (inclusive) of the range.
 * @param {int} max - The upper bound (inclusive) of the range.
 * @returns {int} - A random number in the specified range.
 */
int random_range(int min, int max) {
  if(min >= max)
    return min;

  return random(max - min+1) + min;
}

int system_seed() {
  return _system_seed;
}

void set_system_seed(int seed) {
  _system_seed = seed;
}
