#ifndef __ARMOUR_H__
#define __ARMOUR_H__

#include <clothing.h>

int equip(string slot, object tp) ;
int unequip() ;
public void set_defense(mapping def) ;
public void add_defense(string type, float amount) ;
public mapping query_defense() ;
public float query_defense_amount(string type) ;
public void set_ac(float ac) ;
public float query_ac() ;
public float add_ac(float ac) ;
int is_armour() ;


#endif // __ARMOUR_H__
