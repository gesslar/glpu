#ifndef __ARMOUR_H__
#define __ARMOUR_H__

#include <clothing.h>

public void set_defense(mapping def) ;
public void add_defense(string type, float amount) ;
public mapping query_defense() ;
public float query_defense_amount(string type) ;
int is_armour() ;


#endif // __ARMOUR_H__
