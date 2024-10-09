/**
 * @file /std/object/weight.c
 * @description Handles mass of objects
 *
 * @created 2024-02-18 - Gesslar
 * @last_modified 2024-02-18 - Gesslar
 *
 * @history
 * 2024-02-18 - Gesslar - Created
 */

#include <weight.h>

int _mass ;

int query_mass() {
  return _mass ;
}

int set_mass(int new_mass) {
  int delta ;

  if(new_mass < 0)
    return 0 ;

  // If our new mass is less than 0, we cannot set the mass.
  delta = new_mass - _mass ;

  return adjust_mass(delta) ;
}

int adjust_mass(int delta) {
  object env ;
  int new_mass ;

  // We're not changing mass. So, that's all right.
  if(delta == 0)
    return 1 ;

  // If we have an environment, we need to check if we can adjust its
  // mass and fill and if so, then we can adjust our mass.
  if(env = environment()) {
    if(!env->adjust_mass(delta))
      return 0 ;

    if(!env->adjust_fill(delta)) {
      env->adjust_mass(-delta) ;
      return 0 ;
    }
  }

  _mass += delta ;
  return 1 ;
}
